/**
 * @file
 *
 * @author tomsons26
 *
 * @brief Class representing a big checkbox element.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef BIGCHECK_H
#define BIGCHECK_H

#include "toggle.h"
#include "dialog.h"

class BigCheckBoxClass : public ToggleClass
{
public:
    BigCheckBoxClass(unsigned id, int x, int y, int w, int h, const char *text, TextPrintType style);
    BigCheckBoxClass(BigCheckBoxClass &that);
    virtual ~BigCheckBoxClass();

    virtual BOOL Draw_Me(BOOL redraw) override;
    virtual BOOL Action(unsigned flags, KeyNumType &key) override;

    BigCheckBoxClass & operator=(BigCheckBoxClass &that);

protected:
    TextPrintType m_TextStyle;
    const char *m_TextString;
};

inline BigCheckBoxClass &BigCheckBoxClass::operator=(BigCheckBoxClass &that)
{
    if (this != &that) {
        ToggleClass::operator=(that);
        m_TextStyle = that.m_TextStyle;
        m_TextString = that.m_TextString;
    }

    return *this;
}

#endif // BIGCHECK_H
