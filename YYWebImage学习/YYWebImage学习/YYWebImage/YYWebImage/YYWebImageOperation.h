//
//  YYWebImageOperation.h
//  YYWebImage <https://github.com/ibireme/YYWebImage>
//
//  Created by ibireme on 15/2/15.
//  Copyright (c) 2015 ibireme.
//
//  This source code is licensed under the MIT-style license found in the
//  LICENSE file in the root directory of this source tree.
//

#import <UIKit/UIKit.h>

#if __has_include(<YYWebImage/YYWebImage.h>)
#import <YYWebImage/YYImageCache.h>
#import <YYWebImage/YYWebImageManager.h>
#else
#import "YYImageCache.h"
#import "YYWebImageManager.h"
#endif

NS_ASSUME_NONNULL_BEGIN


/**
 *  NSOperation多线程的子类，请求图片
 
 异步操作。通常，您可以通过增加执行它
 它的操作队列，或调用'start'方法手动执行它。 当。。。的时候
 操作时，它会：
 
 1.从缓存中获取图像，如果存在，用`completion`块返回。
 2.启动一个URL连接从请求获取的图像，调用`progress`
 通知要求进步（而调用`completion`块返回
 如果由渐进式选项启用逐行扫描图像）。
 3.过程通过调用`transform`块图像。
 4.将图像缓存和`completion`块返回。
 */
@interface YYWebImageOperation : NSOperation

// request请求对象
@property (nonatomic, strong, readonly)           NSURLRequest      *request;
// response返回对象
@property (nullable, nonatomic, strong, readonly) NSURLResponse     *response;
// 图片缓存对象
@property (nullable, nonatomic, strong, readonly) YYImageCache      *cache;
// 图片缓存的key
@property (nonatomic, strong, readonly)           NSString          *cacheKey;
// 图片类型
@property (nonatomic, readonly)                   YYWebImageOptions options;

/**
 
 */
@property (nonatomic) BOOL shouldUseCredentialStorage;

/**
 
 */
@property (nullable, nonatomic, strong) NSURLCredential *credential;

/**
 
创建并返回一个YYWebImageOperation对象
 */
- (instancetype)initWithRequest:(NSURLRequest *)request
                        options:(YYWebImageOptions)options
                          cache:(nullable YYImageCache *)cache
                       cacheKey:(nullable NSString *)cacheKey
                       progress:(nullable YYWebImageProgressBlock)progress
                      transform:(nullable YYWebImageTransformBlock)transform
                     completion:(nullable YYWebImageCompletionBlock)completion NS_DESIGNATED_INITIALIZER;

- (instancetype)init UNAVAILABLE_ATTRIBUTE;
+ (instancetype)new UNAVAILABLE_ATTRIBUTE;

@end

NS_ASSUME_NONNULL_END
