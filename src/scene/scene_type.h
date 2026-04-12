#ifndef __SCENE_TAGS_H__
#define __SCENE_TAGS_H__

typedef enum SceneType {
    SCENE_TYPE_UNKNOWN = 0,
    SCENE_TYPE_EMPTY,
    SCENE_TYPE_BLOCK,
    SCENE_TYPE_MAIN_CHAR,
    SCENE_TYPE_MAIN_CHAR_FOOT,
    SCENE_TYPE_COLLIDER
} SceneType;

#endif // __SCENE_TAGS_H__