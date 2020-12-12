#pragma once

#import <Metal/MTLBuffer.h>
#import <Metal/MTLRenderCommandEncoder.h>
#import <Metal/MTLStageInputOutputDescriptor.h>

namespace cc {
namespace gfx {

class CCMTLBuffer;
class CCMTLRenderCommandEncoder;

class CCMTLBuffer : public Buffer {
public:
    CCMTLBuffer(Device *device);
    ~CCMTLBuffer() = default;

    virtual bool initialize(const BufferInfo &info) override;
    virtual bool initialize(const BufferViewInfo &info) override;
    virtual void destroy() override;
    virtual void resize(uint size) override;
    virtual void update(void *buffer, uint offset, uint size) override;

    CC_INLINE id<MTLBuffer> getMTLBuffer() const { return _mtlBuffer; }
    CC_INLINE uint8_t *getTransferBuffer() const { return _transferBuffer; }
    CC_INLINE MTLIndexType getIndexType() const { return _indexType; }
    CC_INLINE bool isDrawIndirectByIndex() const { return _isDrawIndirectByIndex; }
    CC_INLINE const DrawInfoList &getDrawInfos() const { return _indirects; }
    void encodeBuffer(CCMTLRenderCommandEncoder &encoder, uint offset, uint binding, ShaderStageFlags stages);

private:
    void resizeBuffer(uint8_t **, uint, uint);
    bool createMTLBuffer(uint size, MemoryUsage usage);

    id<MTLDevice> _mtlDevice = nil;
    uint _inflightIndex = 0;
    id<MTLBuffer> _mtlBuffer = nullptr;
    uint8_t *_transferBuffer = nullptr;
    MTLIndexType _indexType = MTLIndexTypeUInt16;
    MTLResourceOptions _mtlResourceOptions = MTLResourceStorageModePrivate;
    NSMutableArray *_dynamicDataBuffers = nil;
    bool _isDrawIndirectByIndex = false;
    uint _bufferViewOffset = 0;
    bool _indirectDrawSupported = false;
    DrawInfoList _indirects;
};

} // namespace gfx
} // namespace cc
