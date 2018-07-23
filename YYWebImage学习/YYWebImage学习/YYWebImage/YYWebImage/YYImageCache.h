//
//  YYImageCache.h
//  YYWebImage <https://github.com/ibireme/YYWebImage>
//
//  Created by ibireme on 15/2/15.
//  Copyright (c) 2015 ibireme.
//
//  This source code is licensed under the MIT-style license found in the
//  LICENSE file in the root directory of this source tree.
//

#import <UIKit/UIKit.h>

@class YYMemoryCache, YYDiskCache;

NS_ASSUME_NONNULL_BEGIN

/// Image cache type
typedef NS_OPTIONS(NSUInteger, YYImageCacheType) {
    /// No value.
    YYImageCacheTypeNone   = 0,
    
    /// Get/store image with memory cache.
    /// 从内存中获取图片
    YYImageCacheTypeMemory = 1 << 0,
    
    /// Get/store image with disk cache.
    /// 从磁盘获取图片
    YYImageCacheTypeDisk   = 1 << 1,
    
    /// Get/store image with both memory cache and disk cache.
    YYImageCacheTypeAll    = YYImageCacheTypeMemory | YYImageCacheTypeDisk,
};


/**
 YYImageCache is a cache that stores UIImage and image data based on memory cache and disk cache.
 
 @discussion The disk cache will try to protect the original image data:
 
 * If the original image is still image, it will be saved as png/jpeg file based on alpha information.
 * If the original image is animated gif, apng or webp, it will be saved as original format.
 * If the original image's scale is not 1, the scale value will be saved as extended data.
 
 Although UIImage can be serialized with NSCoding protocol, but it's not a good idea:
 Apple actually use UIImagePNGRepresentation() to encode all kind of image, it may 
 lose the original multi-frame data. The result is packed to plist file and cannot
 view with photo viewer directly. If the image has no alpha channel, using JPEG 
 instead of PNG can save more disk size and encoding/decoding time.
 */
@interface YYImageCache : NSObject

#pragma mark - Attribute
///=============================================================================
/// @name Attribute
///=============================================================================

/** The name of the cache. Default is nil. */
/* 缓存名称*/
@property (nullable, copy) NSString *name;

/** The underlying memory cache. see `YYMemoryCache` for more information.*/
// 内存缓存
@property (strong, readonly) YYMemoryCache *memoryCache;

/** The underlying disk cache. see `YYDiskCache` for more information.*/
// 磁盘缓存
@property (strong, readonly) YYDiskCache *diskCache;

/**
 Whether decode animated image when fetch image from disk cache. Default is YES.
 
 @discussion When fetch image from disk cache, it will use 'YYImage' to decode 
 animated image such as WebP/APNG/GIF. Set to 'NO' to ignore animated image.
 */

@property BOOL allowAnimatedImage;

/**
 Whether decode the image to memory bitmap. Default is YES.
 
 @discussion If the value is YES, then the image will be decoded to memory bitmap
 for better display performance, but may cost more memory.
 */
@property BOOL decodeForDisplay;


#pragma mark - Initializer
///=============================================================================
/// @name Initializer
///=============================================================================
- (instancetype)init UNAVAILABLE_ATTRIBUTE;
+ (instancetype)new UNAVAILABLE_ATTRIBUTE;


// 单例对象
+ (instancetype)sharedCache;


// 根据path初始化
- (nullable instancetype)initWithPath:(NSString *)path NS_DESIGNATED_INITIALIZER;


#pragma mark - Access Methods
///=============================================================================
/// @name Access Methods
///=============================================================================


// 根据key存储一个image
- (void)setImage:(UIImage *)image forKey:(NSString *)key;


- (void)setImage:(nullable UIImage *)image
       imageData:(nullable NSData *)imageData
          forKey:(NSString *)key
        withType:(YYImageCacheType)type;


// 根据key删除image
- (void)removeImageForKey:(NSString *)key;


// 根据key和type删除图片
- (void)removeImageForKey:(NSString *)key withType:(YYImageCacheType)type;


// 根据key判断是否有图片
- (BOOL)containsImageForKey:(NSString *)key;


// 根据key和type判断是否有图片
- (BOOL)containsImageForKey:(NSString *)key withType:(YYImageCacheType)type;


// 根据可以获取image
- (nullable UIImage *)getImageForKey:(NSString *)key;


// 根据key和type获取image
- (nullable UIImage *)getImageForKey:(NSString *)key withType:(YYImageCacheType)type;


- (void)getImageForKey:(NSString *)key
              withType:(YYImageCacheType)type
             withBlock:(void(^)(UIImage * _Nullable image, YYImageCacheType type))block;


- (nullable NSData *)getImageDataForKey:(NSString *)key;


- (void)getImageDataForKey:(NSString *)key
                 withBlock:(void(^)(NSData * _Nullable imageData))block;

@end

NS_ASSUME_NONNULL_END
