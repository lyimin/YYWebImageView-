//
//  YYWebImageManager.h
//  YYWebImage <https://github.com/ibireme/YYWebImage>
//
//  Created by ibireme on 15/2/19.
//  Copyright (c) 2015 ibireme.
//
//  This source code is licensed under the MIT-style license found in the
//  LICENSE file in the root directory of this source tree.
//

#import <UIKit/UIKit.h>

#if __has_include(<YYWebImage/YYWebImage.h>)
#import <YYWebImage/YYImageCache.h>
#else
#import "YYImageCache.h"
#endif

@class YYWebImageOperation;

NS_ASSUME_NONNULL_BEGIN

/// The options to control image operation.
// 用于控制图片的操作
typedef NS_OPTIONS(NSUInteger, YYWebImageOptions) {
    
    /// 下载图片时在状态栏显示网络活动
    YYWebImageOptionShowNetworkActivity = 1 << 0,
    
    /// 下载过程中逐行显示映像（相当于web浏览器的效果）
    YYWebImageOptionProgressive = 1 << 1,
    
    /// 显示模糊渐进式JPEG或下载过程中隔行PNG图像。
    /// 这会忽略更好的用户体验的基准图像。
    YYWebImageOptionProgressiveBlur = 1 << 2,
    
    /// 利用NSURLCache代替YYImageCache。
    YYWebImageOptionUseNSURLCache = 1 << 3,
    
    /// 允许不受信任的SSL证书。
    YYWebImageOptionAllowInvalidSSLCertificates = 1 << 4,
    
    /// 允许当应用程序是在后台后台任务下载图像。
    YYWebImageOptionAllowBackgroundTask = 1 << 5,
    
    YYWebImageOptionHandleCookies = 1 << 6,
    
    /// 加载远程图像并刷新图像缓存。
    YYWebImageOptionRefreshImageCache = 1 << 7,
    
    /// 不要从磁盘缓存加载图像。
    YYWebImageOptionIgnoreDiskCache = 1 << 8,
    
    /// 不要更改视图图像前设置一个新的URL给它。
    YYWebImageOptionIgnorePlaceHolder = 1 << 9,
    
    /// 忽略图像解码。
    /// 这可能用于下载图像无显示。
    YYWebImageOptionIgnoreImageDecoding = 1 << 10,
    
    /// 忽略多帧图象解码。
    /// 这将处理GIF / APNG/的WebP/ ICO图像作为单帧图像。
    YYWebImageOptionIgnoreAnimatedImage = 1 << 11,
    
    /// 设置为查看与动画渐变图像。
    /// 这将增加图像视图层上的“变脸”动画更好的用户体验。
    YYWebImageOptionSetImageWithFadeAnimation = 1 << 12,
    
    /// 不要设置图像时图像的获取完整的视图。
    /// 您可以手动设置图像。
    YYWebImageOptionAvoidSetImage = 1 << 13,
    
    /// 此标志将URL添加到黑名单（在内存中）当URL不能下载，
    /// 所以库将不会继续尝试。
    YYWebImageOptionIgnoreFailedURL = 1 << 14,
};

/// 指明图像是从哪里来的。
typedef NS_ENUM(NSUInteger, YYWebImageFromType) {
    
    /// No value.
    YYWebImageFromNone = 0,
    
    /// 从内存缓存中获取。
    YYWebImageFromMemoryCacheFast,
    YYWebImageFromMemoryCache,
    
    /// 从磁盘内存中获取
    YYWebImageFromDiskCache,
    
    /// 从远程服务器中获取
    YYWebImageFromRemote,
};

/// 图像获取阶段
typedef NS_ENUM(NSInteger, YYWebImageStage) {
    
    /// 不完整，进步的形象。
    YYWebImageStageProgress  = -1,
    
    /// 取消
    YYWebImageStageCancelled = 0,
    
    /// 完成
    YYWebImageStageFinished  = 1,
};


/**
 *  图片下载进度回调
 *
 *  @param receivedSize 当前收到字节大小
 *  @param expectedSize 预计下载总大小（-1 表示未知）
 */
typedef void(^YYWebImageProgressBlock)(NSInteger receivedSize, NSInteger expectedSize);

/*
 这个block是当image下载完成时，对image展示的一些操作，比如动画展示
 */
typedef UIImage * _Nullable (^YYWebImageTransformBlock)(UIImage *image, NSURL *url);

/**
 *  图片请求完成或取消回调block
 *
 *  @param image 图片
 *  @param url   图片url
 *  @param from  图片从哪里获取
 *  @param stage 获取阶段
 *  @param error 错误处理
 */
typedef void (^YYWebImageCompletionBlock)(UIImage * _Nullable image,
                                          NSURL *url,
                                          YYWebImageFromType from,
                                          YYWebImageStage stage,
                                          NSError * _Nullable error);




@interface YYWebImageManager : NSObject

// 单例对象
+ (instancetype)sharedManager;

// 初始化
- (instancetype)initWithCache:(nullable YYImageCache *)cache
                        queue:(nullable NSOperationQueue *)queue NS_DESIGNATED_INITIALIZER;

- (instancetype)init UNAVAILABLE_ATTRIBUTE;
+ (instancetype)new UNAVAILABLE_ATTRIBUTE;

/**
 根据url发送一个图片请求
 */
- (nullable YYWebImageOperation *)requestImageWithURL:(NSURL *)url
                                              options:(YYWebImageOptions)options
                                             progress:(nullable YYWebImageProgressBlock)progress
                                            transform:(nullable YYWebImageTransformBlock)transform
                                           completion:(nullable YYWebImageCompletionBlock)completion;

/**
 通过图像操作中使用的图像缓存。
 您可以将其设置为nil，以避免图像缓存。
 */
@property (nullable, nonatomic, strong) YYImageCache *cache;

// 图片请求队列
@property (nullable, nonatomic, strong) NSOperationQueue *queue;

// 下载完成后，图片展示block
@property (nullable, nonatomic, copy) YYWebImageTransformBlock sharedTransformBlock;


/**
 *  图片请求超时。 默认是15秒
 */
@property (nonatomic) NSTimeInterval timeout;

/**
 The username used by NSURLCredential, default is nil.
 */
@property (nullable, nonatomic, copy) NSString *username;

/**
 The password used by NSURLCredential, default is nil.
 */
@property (nullable, nonatomic, copy) NSString *password;

/**
 *  图片http请求头部。 默认是"Accept:image/webp,image/\*;q=0.8"
 */
@property (nullable, nonatomic, copy) NSDictionary<NSString *, NSString *> *headers;


@property (nullable, nonatomic, copy) NSDictionary<NSString *, NSString *> *(^headersFilter)(NSURL *url, NSDictionary<NSString *, NSString *> * _Nullable header);

@property (nullable, nonatomic, copy) NSString *(^cacheKeyFilter)(NSURL *url);

// 返回图片请求的header
- (nullable NSDictionary<NSString *, NSString *> *)headersForURL:(NSURL *)url;

// 根据url返回图片缓存的key
- (NSString *)cacheKeyForURL:(NSURL *)url;


/**
 Increments the number of active network requests.
 If this number was zero before incrementing, this will start animating the
 status bar network activity indicator.
 
 This method is thread safe.
 
 This method has no effect in App Extension.
 */
+ (void)incrementNetworkActivityCount;

/**
 Decrements the number of active network requests.
 If this number becomes zero after decrementing, this will stop animating the
 status bar network activity indicator.
 
 This method is thread safe.
 
 This method has no effect in App Extension.
 */
+ (void)decrementNetworkActivityCount;

// 当前图片请求的数量
+ (NSInteger)currentNetworkActivityCount;

@end

NS_ASSUME_NONNULL_END
