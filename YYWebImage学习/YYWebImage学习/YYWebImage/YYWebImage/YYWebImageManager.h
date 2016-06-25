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
    
    /// Show network activity on status bar when download image.
    /// 下载图片时在状态栏显示网络活动
    YYWebImageOptionShowNetworkActivity = 1 << 0,
    
    /// 下载过程中逐行显示映像（相当于web浏览器的效果）
    /// Display progressive/interlaced/baseline image during download (same as web browser).
    YYWebImageOptionProgressive = 1 << 1,
    
    /// Display blurred progressive JPEG or interlaced PNG image during download.
    /// This will ignore baseline image for better user experience.
    /// 显示模糊渐进式JPEG或下载过程中隔行PNG图像。
    /// 这会忽略更好的用户体验的基准图像。
    YYWebImageOptionProgressiveBlur = 1 << 2,
    
    /// Use NSURLCache instead of YYImageCache.
    /// 利用NSURLCache代替YYImageCache。
    YYWebImageOptionUseNSURLCache = 1 << 3,
    
    /// Allows untrusted SSL ceriticates.
    /// 允许不受信任的SSL证书。
    YYWebImageOptionAllowInvalidSSLCertificates = 1 << 4,
    
    /// Allows background task to download image when app is in background.
    /// 允许当应用程序是在后台后台任务下载图像。
    YYWebImageOptionAllowBackgroundTask = 1 << 5,
    
    /// Handles cookies stored in NSHTTPCookieStore.
    YYWebImageOptionHandleCookies = 1 << 6,
    
    /// Load the image from remote and refresh the image cache.
    /// 加载远程图像并刷新图像缓存。
    YYWebImageOptionRefreshImageCache = 1 << 7,
    
    /// Do not load image from/to disk cache.
    /// 不要从磁盘缓存加载图像。
    YYWebImageOptionIgnoreDiskCache = 1 << 8,
    
    /// Do not change the view's image before set a new URL to it.
    /// 不要更改视图图像前设置一个新的URL给它。
    YYWebImageOptionIgnorePlaceHolder = 1 << 9,
    
    /// Ignore image decoding.
    /// This may used for image downloading without display.
    /// 忽略图像解码。
    /// 这可能用于下载图像无显示。
    YYWebImageOptionIgnoreImageDecoding = 1 << 10,
    
    /// Ignore multi-frame image decoding.
    /// This will handle the GIF/APNG/WebP/ICO image as single frame image.
    /// 忽略多帧图象解码。
    /// 这将处理GIF / APNG/的WebP/ ICO图像作为单帧图像。
    YYWebImageOptionIgnoreAnimatedImage = 1 << 11,
    
    /// Set the image to view with a fade animation.
    /// This will add a "fade" animation on image view's layer for better user experience.
    /// 设置为查看与动画渐变图像。
    /// 这将增加图像视图层上的“变脸”动画更好的用户体验。
    YYWebImageOptionSetImageWithFadeAnimation = 1 << 12,
    
    /// Do not set the image to the view when image fetch complete.
    /// You may set the image manually.
    /// 不要设置图像时图像的获取完整的视图。
    /// 您可以手动设置图像。
    YYWebImageOptionAvoidSetImage = 1 << 13,
    
    /// This flag will add the URL to a blacklist (in memory) when the URL fail to be downloaded,
    /// so the library won't keep trying.
    /// 此标志将URL添加到黑名单（在内存中）当URL不能下载，
    /// 所以库将不会继续尝试。
    YYWebImageOptionIgnoreFailedURL = 1 << 14,
};

/// Indicated where the image came from.
/// 指明图像是从哪里来的。
typedef NS_ENUM(NSUInteger, YYWebImageFromType) {
    
    /// No value.
    YYWebImageFromNone = 0,
    
    /// Fetched from memory cache immediately.
    /// If you called "setImageWithURL:..." and the image is already in memory,
    /// then you will get this value at the same call.
    
    /// 从内存缓存中获取。
    YYWebImageFromMemoryCacheFast,
    
    /// Fetched from memory cache.
    YYWebImageFromMemoryCache,
    
    /// Fetched from disk cache.
    /// 从磁盘内存中获取
    YYWebImageFromDiskCache,
    
    /// Fetched from remote (web or file path).
    /// 从远程服务器中获取
    YYWebImageFromRemote,
};

/// Indicated image fetch complete stage.
/// 图像获取阶段
typedef NS_ENUM(NSInteger, YYWebImageStage) {
    
    /// Incomplete, progressive image.
    /// 不完整，进步的形象。
    YYWebImageStageProgress  = -1,
    
    /// Cancelled.
    /// 取消
    YYWebImageStageCancelled = 0,
    
    /// Finished (succeed or failed).
    /// 完成
    YYWebImageStageFinished  = 1,
};


/**
 The block invoked in remote image fetch progress.
 
 @param receivedSize Current received size in bytes.
 @param expectedSize Expected total size in bytes (-1 means unknown).
 */
/**
 *  图片下载进度回调
 *
 *  @param receivedSize 当前收到字节大小
 *  @param expectedSize 预计下载总大小（-1 表示未知）
 */
typedef void(^YYWebImageProgressBlock)(NSInteger receivedSize, NSInteger expectedSize);

/**
 The block invoked before remote image fetch finished to do additional image process.
 
 @discussion This block will be invoked before `YYWebImageCompletionBlock` to give
 you a chance to do additional image process (such as resize or crop). If there's
 no need to transform the image, just return the `image` parameter.
 
 @example You can clip the image, blur it and add rounded corners with these code:
    ^(UIImage *image, NSURL *url) {
        // Maybe you need to create an @autoreleasepool to limit memory cost.
        image = [image yy_imageByResizeToSize:CGSizeMake(100, 100) contentMode:UIViewContentModeScaleAspectFill];
        image = [image yy_imageByBlurRadius:20 tintColor:nil tintMode:kCGBlendModeNormal saturation:1.2 maskImage:nil];
        image = [image yy_imageByRoundCornerRadius:5];
        return image;
    }
 
 @param image The image fetched from url.
 @param url   The image url (remote or local file path).
 @return The transformed image.
 */
typedef UIImage * _Nullable (^YYWebImageTransformBlock)(UIImage *image, NSURL *url);

/**
 The block invoked when image fetch finished or cancelled.
 
 @param image       The image.
 @param url         The image url (remote or local file path).
 @param from        Where the image came from.
 @param error       Error during image fetching.
 @param finished    If the operation is cancelled, this value is NO, otherwise YES.
 */
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




/**
 A manager to create and manage web image operation.
 */
@interface YYWebImageManager : NSObject

/**
 Returns global YYWebImageManager instance.
 
 @return YYWebImageManager shared instance.
 */
+ (instancetype)sharedManager;

/**
 Creates a manager with an image cache and operation queue.
 
 @param cache  Image cache used by manager (pass nil to avoid image cache).
 @param queue  The operation queue on which image operations are scheduled and run
                (pass nil to make the new operation start immediately without queue).
 @return A new manager.
 */
- (instancetype)initWithCache:(nullable YYImageCache *)cache
                        queue:(nullable NSOperationQueue *)queue NS_DESIGNATED_INITIALIZER;

- (instancetype)init UNAVAILABLE_ATTRIBUTE;
+ (instancetype)new UNAVAILABLE_ATTRIBUTE;

/**
 Creates and returns a new image operation, the operation will start immediately.
 创建并返回一个新的图像操作，该操作将立即启动。
 
 @param url        The image url (remote or local file path).
 @param options    The options to control image operation.
 @param progress   Progress block which will be invoked on background thread (pass nil to avoid).
 @param transform  Transform block which will be invoked on background thread  (pass nil to avoid).
 @param completion Completion block which will be invoked on background thread  (pass nil to avoid).
 @return A new image operation.
 */
- (nullable YYWebImageOperation *)requestImageWithURL:(NSURL *)url
                                              options:(YYWebImageOptions)options
                                             progress:(nullable YYWebImageProgressBlock)progress
                                            transform:(nullable YYWebImageTransformBlock)transform
                                           completion:(nullable YYWebImageCompletionBlock)completion;

/**
 The image cache used by image operation. 
 You can set it to nil to avoid image cache.
 通过图像操作中使用的图像缓存。
 您可以将其设置为nil，以避免图像缓存。
 */
@property (nullable, nonatomic, strong) YYImageCache *cache;

/**
 The operation queue on which image operations are scheduled and run.
 You can set it to nil to make the new operation start immediately without queue.
 
 You can use this queue to control maximum number of concurrent operations, to obtain 
 the status of the current operations, or to cancel all operations in this manager.
 */
@property (nullable, nonatomic, strong) NSOperationQueue *queue;

/**
 The shared transform block to process image. Default is nil.
 
 When called `requestImageWithURL:options:progress:transform:completion` and
 the `transform` is nil, this block will be used.
 */
@property (nullable, nonatomic, copy) YYWebImageTransformBlock sharedTransformBlock;

/**
 The image request timeout interval in seconds. Default is 15.
 */
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
 The image HTTP request header. Default is "Accept:image/webp,image/\*;q=0.8".
 */
/**
 *  图片http请求头部。 默认是"Accept:image/webp,image/\*;q=0.8"
 */
@property (nullable, nonatomic, copy) NSDictionary<NSString *, NSString *> *headers;

/**
 A block which will be invoked for each image HTTP request to do additional
 HTTP header process. Default is nil.
 
 Use this block to add or remove HTTP header field for a specified URL.
 */
@property (nullable, nonatomic, copy) NSDictionary<NSString *, NSString *> *(^headersFilter)(NSURL *url, NSDictionary<NSString *, NSString *> * _Nullable header);

/**
 A block which will be invoked for each image operation. Default is nil.
 
 Use this block to provide a custom image cache key for a specified URL.
 */
@property (nullable, nonatomic, copy) NSString *(^cacheKeyFilter)(NSURL *url);

/**
 Returns the HTTP headers for a specified URL.
 
 @param url A specified URL.
 @return HTTP headers.
 */
- (nullable NSDictionary<NSString *, NSString *> *)headersForURL:(NSURL *)url;

/**
 Returns the cache key for a specified URL.
 
 @param url A specified URL
 @return Cache key used in YYImageCache.
 */
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

/**
 Get current number of active network requests.
 
 This method is thread safe.
 
 This method has no effect in App Extension.
 */
+ (NSInteger)currentNetworkActivityCount;

@end

NS_ASSUME_NONNULL_END
