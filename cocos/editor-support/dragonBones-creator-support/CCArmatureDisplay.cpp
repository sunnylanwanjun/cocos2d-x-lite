#include "dragonBones-creator-support/CCArmatureDisplay.h"
#include "dragonBones-creator-support/CCSlot.h"
USING_NS_CC;

DRAGONBONES_NAMESPACE_BEGIN

CCArmatureDisplay* CCArmatureDisplay::create()
{
    CCArmatureDisplay* displayContainer = new (std::nothrow) CCArmatureDisplay();
    if (displayContainer)
    {
        displayContainer->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(displayContainer);
    }
    return displayContainer;
}

CCArmatureDisplay::CCArmatureDisplay()
{
    
}

CCArmatureDisplay::~CCArmatureDisplay()
{
    dispose();
    for (auto it : _listenerIDMap) {
        std::vector<uint32_t>& idArr = *(it.second);
        for (auto i : idArr) {
            EventDispatcher::removeCustomEventListener(it.first, i);
        }
        delete &idArr;
    }
    if(_parent){
        _parent->removeChild(this);
    }
}

void CCArmatureDisplay::removeChild(CCArmatureDisplay* child){
    auto it = _children.find(child);
    if (it != _children.end()) {
        _children.erase(it);
    }
}

void CCArmatureDisplay::addChild(CCArmatureDisplay* child){
    _children.pushBack(child);
}

void CCArmatureDisplay::dbInit(Armature* armature)
{
    _armature = armature;
}

void CCArmatureDisplay::dbClear()
{
    _armature = nullptr;
    release();
}

void CCArmatureDisplay::dispose(bool disposeProxy)
{
    if (_armature != nullptr) 
    {
        _armature->dispose();
        _armature = nullptr;
    }
}

void CCArmatureDisplay::dbUpdate()
{
    const auto drawed = DragonBones::debugDraw;
    if (drawed || _debugDraw) 
    {
        _debugDraw = drawed;
        if (_debugDraw) 
        {

        }
        else 
        {
            // TODO
        }
    }
}

void CCArmatureDisplay::addDBEventListener(const std::string& type, const std::function<void(EventObject*)>& callback)
{
    auto lambda = [callback](const CustomEvent& event) -> void
    {
        callback(static_cast<EventObject*>(event.args[0].ptrVal));
    };
    uint32_t listenerID = EventDispatcher::addCustomEventListener(type, lambda);
    auto it = _listenerIDMap.find(type);
    if (it != _listenerIDMap.end()) {
        auto idArr = it->second;
        idArr->push_back(listenerID);
    } else {
        auto idArr = new std::vector<uint32_t>;
        idArr->push_back(listenerID);
        _listenerIDMap[type] = idArr;
    }
}

void CCArmatureDisplay::dispatchDBEvent(const std::string& type, EventObject* value)
{
    CustomEvent customEvent;
    customEvent.name = type;
    customEvent.args[0].ptrVal = (void*)value;
    EventDispatcher::dispatchCustomEvent(customEvent);
}

void CCArmatureDisplay::removeDBEventListener(const std::string& type, const std::function<void(EventObject*)>& callback)
{
    auto it = _listenerIDMap.find(type);
    if (it == _listenerIDMap.end()) {
        return;
    }
    std::vector<uint32_t>& idArr = *(it->second);
    for (auto i : idArr) {
        EventDispatcher::removeCustomEventListener(type, i);
    }
    delete &idArr;
    _listenerIDMap.erase(it);
}

/*
DBCCSprite* DBCCSprite::create()
{
    DBCCSprite* sprite = new (std::nothrow) DBCCSprite();

    if (sprite && sprite->init())
    {
        sprite->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(sprite);
    }

    return sprite;
}

bool DBCCSprite::_checkVisibility(const cocos2d::Mat4& transform, const cocos2d::Size& size, const cocos2d::Rect& rect)
{
    auto scene = cocos2d::Director::getInstance()->getRunningScene();

    //If draw to Rendertexture, return true directly.
    // only cull the default camera. The culling algorithm is valid for default camera.
    if (!scene || (scene && scene->getDefaultCamera() != cocos2d::Camera::getVisitingCamera()))
        return true;

    auto director = cocos2d::Director::getInstance();
    cocos2d::Rect visiableRect(director->getVisibleOrigin(), director->getVisibleSize());

    // transform center point to screen space
    float hSizeX = size.width / 2;
    float hSizeY = size.height / 2;

    cocos2d::Vec3 v3p(hSizeX, hSizeY, 0);

    transform.transformPoint(&v3p);
    cocos2d::Vec2 v2p = cocos2d::Camera::getVisitingCamera()->projectGL(v3p);

    // convert content size to world coordinates
    float wshw = std::max(fabsf(hSizeX * transform.m[0] + hSizeY * transform.m[4]), fabsf(hSizeX * transform.m[0] - hSizeY * transform.m[4]));
    float wshh = std::max(fabsf(hSizeX * transform.m[1] + hSizeY * transform.m[5]), fabsf(hSizeX * transform.m[1] - hSizeY * transform.m[5]));

    // enlarge visible rect half size in screen coord
    visiableRect.origin.x -= wshw;
    visiableRect.origin.y -= wshh;
    visiableRect.size.width += wshw * 2;
    visiableRect.size.height += wshh * 2;
    bool ret = visiableRect.containsPoint(v2p);
    return ret;
}

void DBCCSprite::draw(cocos2d::Renderer* renderer, const cocos2d::Mat4& transform, uint32_t flags)
{
#if CC_USE_CULLING
#if COCOS2D_VERSION >= 0x00031400
    const auto& rect = _polyInfo.getRect();
#else
    const auto& rect = _polyInfo.rect;
#endif
    
    // Don't do calculate the culling if the transform was not updated
    auto visitingCamera = cocos2d::Camera::getVisitingCamera();
    auto defaultCamera = cocos2d::Camera::getDefaultCamera();
    if (visitingCamera == defaultCamera) {
        _insideBounds = ((flags & FLAGS_TRANSFORM_DIRTY) || visitingCamera->isViewProjectionUpdated()) ? _checkVisibility(transform, _contentSize, rect) : _insideBounds;
    }
    else
    {
        _insideBounds = _checkVisibility(transform, _contentSize, rect);
    }

    if (_insideBounds)
#endif
    {
        _trianglesCommand.init(_globalZOrder, _texture->getName(), getGLProgramState(), _blendFunc, _polyInfo.triangles, transform, flags);
        renderer->addCommand(&_trianglesCommand);

#if CC_SPRITE_DEBUG_DRAW
        _debugDrawNode->clear();
        auto count = _polyInfo.triangles.indexCount / 3;
        auto indices = _polyInfo.triangles.indices;
        auto verts = _polyInfo.triangles.verts;
        for (ssize_t i = 0; i < count; i++)
        {
            //draw 3 lines
            auto from = verts[indices[i * 3]].vertices;
            auto to = verts[indices[i * 3 + 1]].vertices;
            _debugDrawNode->drawLine(cocos2d::Vec2(from.x, from.y), cocos2d::Vec2(to.x, to.y), cocos2d::Color4F::WHITE);

            from = verts[indices[i * 3 + 1]].vertices;
            to = verts[indices[i * 3 + 2]].vertices;
            _debugDrawNode->drawLine(cocos2d::Vec2(from.x, from.y), cocos2d::Vec2(to.x, to.y), cocos2d::Color4F::WHITE);

            from = verts[indices[i * 3 + 2]].vertices;
            to = verts[indices[i * 3]].vertices;
            _debugDrawNode->drawLine(cocos2d::Vec2(from.x, from.y), cocos2d::Vec2(to.x, to.y), cocos2d::Color4F::WHITE);
        }
#endif //CC_SPRITE_DEBUG_DRAW
    }
}

cocos2d::PolygonInfo& DBCCSprite::getPolygonInfoModify()
{
    return _polyInfo;
}
*/

DRAGONBONES_NAMESPACE_END
