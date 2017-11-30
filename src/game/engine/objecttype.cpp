/**
 * @file
 *
 * @author CCHyper
 * @author OmniBlade
 *
 * @brief Base class for most ingame object types.
 *
 * @copyright Redalert++ is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "objecttype.h"
#include "ccfileclass.h"
#include "mixfile.h"

#ifndef RAPP_STANDALONE
void *&ObjectTypeClass::SelectShapes = Make_Global<void *>(0x006857A8);
void *&ObjectTypeClass::PipShapes = Make_Global<void *>(0x006857AC);
#else
void *ObjectTypeClass::SelectShapes = nullptr;
void *ObjectTypeClass::PipShapes = nullptr;
#endif

ObjectTypeClass::ObjectTypeClass(RTTIType type, int id, BOOL animates, BOOL radar_invisible, BOOL selectable,
    BOOL legal_target, BOOL insignificant, BOOL is_immune, BOOL logical, int uiname, const char *name) :
    AbstractTypeClass(type, id, uiname, name),
    Crushable(false),
    RadarInvisible(radar_invisible),
    Selectable(selectable),
    LegalTarget(legal_target),
    Insignificant(insignificant),
    Immune(is_immune),
    Animates(animates),
    Logical(logical),
    Armor(ARMOR_NONE),
    ImageData(nullptr),
    FrameDimensions(0),
    RadarIconData(nullptr)
{
}

ObjectTypeClass::ObjectTypeClass(ObjectTypeClass const &that) :
    AbstractTypeClass(that),
    Crushable(that.Crushable),
    RadarInvisible(that.RadarInvisible),
    Selectable(that.Selectable),
    LegalTarget(that.LegalTarget),
    Insignificant(that.Insignificant),
    Immune(that.Immune),
    Animates(that.Animates),
    Logical(that.Logical),
    Armor(that.Armor),
    Strength(that.Strength),
    ImageData(that.ImageData),
    FrameDimensions(that.FrameDimensions),
    RadarIconData(that.RadarIconData)
{
    memcpy(ImageName, that.ImageName, sizeof(ImageName));
}

ObjectTypeClass::~ObjectTypeClass()
{
    ImageData = nullptr;
    RadarIconData = nullptr;
}

void ObjectTypeClass::Dimensions(int &w, int &h) const
{
    w = 10;
    h = 10;
}

int16_t *ObjectTypeClass::Occupy_List(BOOL a1) const
{
    static int16_t _list[] = { 0, INT16_MAX };

    return _list;
}

int16_t *ObjectTypeClass::Overlap_List() const
{
    static int16_t _list[] = { 0, INT16_MAX };

    return _list;
}

BuildingClass *ObjectTypeClass::Who_Can_Build_Me(BOOL a1, BOOL a2, HousesType house) const
{
    // TODO Requires BuildingClass, BuildingTypeClass, HouseClass
#ifndef RAPP_STANDALONE
    static BuildingClass *(*func)(const ObjectTypeClass *, BOOL, BOOL, HousesType) =
        reinterpret_cast<BuildingClass *(*)(const ObjectTypeClass *, BOOL, BOOL, HousesType)>(0x0051E988);
    return func(this, a1, a2, house);
#else
    return nullptr;
#endif
}

void ObjectTypeClass::One_Time()
{
    SelectShapes = MixFileClass<CCFileClass>::Retrieve("SELECT.SHP");
    PipShapes = MixFileClass<CCFileClass>::Retrieve("PIPS.SHP");
}