/*
 * $Id: Timecode.h,v 1.6 2010/10/12 17:42:25 john_f Exp $
 *
 * Class to hold a Timecode
 *
 * Copyright (C) 2006 - 2010  British Broadcasting Corporation.
 * All Rights Reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 */

#ifndef Timecode_h
#define Timecode_h

#include <sys/timeb.h>
#include <string>

namespace Ingex
{

class Duration
{
public:
    Duration(int frames = 0, int fps_num = 25, int fps_den = 1, bool df = false);
    int FrameCount() { return mFrameCount; }
    const char * Text() const { return mText; }
private:
// fundamental data
    int mFrameCount;
    int mFrameRateNumerator;
    int mFrameRateDenominator;
    bool mDropFrame;

// derived data
    int mHours;
    int mMinutes;
    int mSeconds;
    int mFrames;
    char mText[16];

// methods
    void UpdateHoursMinsEtc();
    void UpdateText();
};

class Timecode
{
public:
// supported timecode formats
    enum FormatEnumType
    {
        NONE,
        TC_25,
        TC_29DF,
        TC_29NDF,
        TC_50,
        TC_59DF,
        TC_59NDF
    };
// constructors
    Timecode();
    Timecode(int frames_since_midnight, int fps_num, int fps_den, bool df);
    Timecode(int frames_since_midnight, FormatEnumType format);
    Timecode(int hr, int min, int sec, int frame, int frame_of_pair, int fps_num, int fps_den, bool df);
    Timecode(int hr, int min, int sec, int frame, int frame_of_pair, FormatEnumType format);
    Timecode(const char * s, int fps_num, int fps_den, bool df);
    Timecode(const char * s, FormatEnumType format);
// copy constructor
    Timecode(const Timecode & tc);
// assignment
    Timecode & operator=(const Timecode &);

// static method
    static FormatEnumType GetFormat(int fps_num, int fps_den, bool df);

// methods
    const char * Text() const { return mText; }
    const char * TextNoSeparators() const { return mTextNoSeparators; }
    int FramesSinceMidnight() const { return mFramesSinceMidnight; }
    //int FrameRateNumerator() const { return mFrameRateNumerator; }
    //int FrameRateDenominator() const { return mFrameRateDenominator; }
    int FrameRateNumerator() const;
    int FrameRateDenominator() const;
    //bool DropFrame() const { return mDropFrame; }
    //void DropFrame(bool df) { mDropFrame = df; UpdateHoursMinsEtc(); }
    bool DropFrame() const;
    int Hours() const { return mHours; }
    int Minutes() const { return mMinutes; }
    int Seconds() const { return mSeconds; }
    int Frames() const { return mFrames; }
    bool IsNull() const { return NONE == mFormat; }
    int NominalFps();
    const char * FormatName() const;

// operator overload
    friend Timecode operator+(const Timecode & lhs, const int & frames);
    friend Duration operator-(const Timecode & lhs, const Timecode & rhs);
    //friend void operator+=(Timecode & lhs, const Timecode & rhs);
    //friend void operator-=(Timecode & lhs, const Timecode & rhs);
    void operator+=(int frames);
    void operator-=(int frames);

private:
// fundamental data
    FormatEnumType mFormat;
    int mFramesSinceMidnight;
    //int mFrameRateNumerator;
    //int mFrameRateDenominator;
    //bool mDropFrame;
    //bool mIsNull;

// derived data
    int mHours;
    int mMinutes;
    int mSeconds;
    int mFrames;
    int mFrameOfPair;
    char mText[16];
    char mTextNoSeparators[16];

// methods
    void UpdateFramesSinceMidnight();
    void UpdateHoursMinsEtc();
    void UpdateText();
    void CheckFramesSinceMidnight();
};

} // namespace

#endif //#ifndef Timecode_h

