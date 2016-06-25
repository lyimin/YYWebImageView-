//
//  _YYWebImageSetter.m
//  YYWebImage <https://github.com/ibireme/YYWebImage>
//
//  Created by ibireme on 15/7/15.
//  Copyright (c) 2015 ibireme.
//
//  This source code is licensed under the MIT-style license found in the
//  LICENSE file in the root directory of this source tree.
//

#import "_YYWebImageSetter.h"
#import "YYWebImageOperation.h"
#import <libkern/OSAtomic.h>
/**
 *  设置动画可以
 *  动画时间
 */

NSString *const _YYWebImageFadeAnimationKey = @"YYWebImageFade";
const NSTimeInterval _YYWebImageFadeTime = 0.2;
const NSTimeInterval _YYWebImageProgressiveFadeTime = 0.4;


@implementation _YYWebImageSetter {
    // 用信号实现线程锁
    dispatch_semaphore_t _lock;
    NSURL *_imageURL;
    NSOperation *_operation;
    int32_t _sentinel;
}
#warning 看到这里
- (instancetype)init {
    self = [super init];
    // 创建信号，设置信号量为1
    _lock = dispatch_semaphore_create(1);
    return self;
}

// 资源访问控制 线程安全
- (NSURL *)imageURL {
    dispatch_semaphore_wait(_lock, DISPATCH_TIME_FOREVER);
    NSURL *imageURL = _imageURL;
    dispatch_semaphore_signal(_lock);
    return imageURL;
}

- (void)dealloc {
    OSAtomicIncrement32(&_sentinel);
    [_operation cancel];
}

/// 创建Web图像新的操作，并返回一个标记值。
- (int32_t)setOperationWithSentinel:(int32_t)sentinel
                                url:(NSURL *)imageURL
                            options:(YYWebImageOptions)options
                            manager:(YYWebImageManager *)manager
                           progress:(YYWebImageProgressBlock)progress
                          transform:(YYWebImageTransformBlock)transform
                         completion:(YYWebImageCompletionBlock)completion {
    if (sentinel != _sentinel) {
        if (completion) completion(nil, imageURL, YYWebImageFromNone, YYWebImageStageCancelled, nil);
        return _sentinel;
    }
    
    // 下载图片
    NSOperation *operation = [manager requestImageWithURL:imageURL options:options progress:progress transform:transform completion:completion];
    if (!operation && completion) {
        NSDictionary *userInfo = @{ NSLocalizedDescriptionKey : @"YYWebImageOperation create failed." };
        completion(nil, imageURL, YYWebImageFromNone, YYWebImageStageFinished, [NSError errorWithDomain:@"com.ibireme.webimage" code:-1 userInfo:userInfo]);
    }
    
    dispatch_semaphore_wait(_lock, DISPATCH_TIME_FOREVER);
    if (sentinel == _sentinel) {
        if (_operation) [_operation cancel];
        _operation = operation;
        sentinel = OSAtomicIncrement32(&_sentinel);
        
    } else {
        [operation cancel];
    }
    dispatch_semaphore_signal(_lock);
    return sentinel;
}

/**
 *  取消
 */
- (int32_t)cancel {
    return [self cancelWithNewURL:nil];
}

/**
 *  取消并设置新的url
 *
 *  @param imageURL 图片url
 *
 *  @return 返回当前
 */
- (int32_t)cancelWithNewURL:(NSURL *)imageURL {
    int32_t sentinel;
    dispatch_semaphore_wait(_lock, DISPATCH_TIME_FOREVER);
    if (_operation) {
        [_operation cancel];
        _operation = nil;
    }
    _imageURL = imageURL;
    sentinel = OSAtomicIncrement32(&_sentinel);
    dispatch_semaphore_signal(_lock);
    return sentinel;
}

+ (dispatch_queue_t)setterQueue {
    static dispatch_queue_t queue;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        queue = dispatch_queue_create("com.ibireme.webimage.setter", DISPATCH_QUEUE_SERIAL);
        dispatch_set_target_queue(queue, dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0));
    });
    return queue;
}

@end
