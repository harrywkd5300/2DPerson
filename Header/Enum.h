#pragma once

enum eObjectType
{
	OBJ_TYPE_BACKGROUND,
	OBJ_TYPE_PLAYER,
	OBJ_TYPE_UNIT,
	OBJ_TYPE_MONSTER,
	OBJ_TYPE_OBJECT,
	OBJ_TYPE_UI,
	OBJ_TYPE_INVEN,
	OBJ_TYPE_PORTRAIT,
	OBJ_TYPE_MOUSE,
	OBJ_TYPE_EFFECT,
	OBJ_TYPE_MAX
};

enum eRenderType
{
	RENDER_TYPE_BACKGROUND,
	RENDER_TYPE_OBJECT,
	RENDER_TYPE_UI,
	RENDER_TYPE_MOUSE,
	RENDER_TYPE_EFFECT,
	RENDER_TYPE_MAX
};

enum eTextureType
{
	TEX_SINGLE,
	TEX_MULTI,
	TEX_MAX,
};
enum eItemType
{
	ITEM_TYPE_WEAPON,
	ITEM_TYPE_ARMOR,
	ITEM_TYPE_HEADER,
	ITEM_TYPE_ACCE,
	ITEM_TYPE_ACCE1,
	ITEM_TYPE_POTION,
	ITEM_TYPE_END,
};

enum eSceneType
{
	SCENE_LOGO,
	SCENE_STAGE,
	SCENE_CITY,
	SCENE_BATTLE,
	SCENE_MAX,
};

enum eWayType
{
	WAY_RIGTH,
	WAY_UP,
	WAY_LEFT,
	WAY_DOWN,
	WAY_RIGTHUP,
	WAY_RIGTHDOWN,
	WAY_LEFTUP,
	WAY_LEFTDOWN,
	WAY_END,
};