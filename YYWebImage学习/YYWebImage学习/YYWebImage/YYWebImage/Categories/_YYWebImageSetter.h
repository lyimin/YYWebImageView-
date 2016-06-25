//
//  _YYWebImageSetter.h
//  YYWebImage <https://github.com/ibireme/YYWebImage>
//
//  Created by ibireme on 15/7/15.
//  Copyright (c) 2015 ibireme.
//
//  This source code is licensed under the MIT-style license found in the
//  LICENSE file in the root directory of this source tree.
//
#import <UIKit/UIKit.h>
#import <pthread.h>

#if __has_include(<YYWebImage/YYWebImage.h>)
#import <YYWebImage/YYWebImageManager.h>
#else
#import "YYWebImageManager.h"
#endif

NS_ASSUME_NONNULL_BEGIN


/**
 *  主队列执行块
 */
static inline void _yy_dispatch_sync_on_main_queue(void (^block)()) {
    if (pthread_main_np()) {
        block();
    } else {
        dispatch_sync(dispatch_get_main_queue(), block);
    }
}
/**
 *  淡入淡出式加载动画key
 */
extern NSString *const _YYWebImageFadeAnimationKey;
/**
 *  web图片淡入淡出式加载时间，默认0.2
 */
extern const NSTimeInterval _YYWebImageFadeTime;
/**
 *  web 图片进度淡入淡出加载时间， 默认0.4
 */
extern const NSTimeInterval _YYWebImageProgressiveFadeTime;

/**
 Private class used by web image categories.
 Typically, you should not use this class directly.
 */

/**
 *
 通过Web图像类别中使用私有类。
 通常情况下，你不应该直接使用此类。
 */
@interface _YYWebImageSetter : NSObject
/// Current image url.
/// 图片url
@property (nullable, nonatomic, readonly) NSURL *imageURL;
/// Current sentinel.
/// 当前定点
@property (nonatomic, readonly) int32_t sentinel;

/// Create new operation for web image and return a sentinel value.
/// 创建Web图像新的操作，并返回一个标记值。
- (int32_t)setOperationWithSentinel:(int32_t)sentinel
                                url:(nullable NSURL *)imageURL
                            options:(YYWebImageOptions)options
                            manager:(YYWebImageManager *)manager
                           progress:(nullable YYWebImageProgressBlock)progress
                          transform:(nullable YYWebImageTransformBlock)transform
                         completion:(nullable YYWebImageCompletionBlock)completion;

/// Cancel and return a sentinel value. The imageURL will be set to nil.
/// 取消，并返回一个标记值。该imageURL将被设为零。
- (int32_t)cancel;

/// Cancel and return a sentinel value. The imageURL will be set to new value.
/// 取消，并返回一个标记值。该imageURL将被设置为新的值。
- (int32_t)cancelWithNewURL:(nullable NSURL *)imageURL;

/// A queue to set operation.
/// 设置队列
+ (dispatch_queue_t)setterQueue;

@end

NS_ASSUME_NONNULL_END
