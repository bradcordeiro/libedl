/*
 * $Id: Timecode.cpp,v 1.13 2011/10/11 11:02:26 john_f Exp $
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
#include <cstring>
#include <cstdio>

#ifdef _MSC_VER

typedef unsigned char uint8_t;
typedef signed char int8_t;

typedef unsigned short uint16_t;
typedef short int16_t;

typedef unsigned int uint32_t;
typedef int int32_t;

typedef unsigned __int64 uint64_t;
typedef __int64 int64_t;

#else
#include <inttypes.h>
#endif

#include "Timecode.h"

// constants for 30 fps drop frame
const unsigned int DF_FRAMES_PER_MINUTE       = 60 * 30 - 2;                    // 1798
const unsigned int DF_FRAMES_PER_TEN_MINUTE   = 10 * DF_FRAMES_PER_MINUTE + 2;  // 17982
const unsigned int DF_FRAMES_PER_HOUR         = 6 * DF_FRAMES_PER_TEN_MINUTE;   // 107892

using namespace Ingex;

// default constructor initialises null timecode
Timecode::Timecode()
: mFormat(NONE), mFramesSinceMidnight(0),
  mHours(0), mMinutes(0), mSeconds(0), mFrames(0), mFrameOfPair(0)
{
    UpdateText();
}


// constructor from frame count and frame rate parameters
Timecode::Timecode(int frames_since_midnight, int fps_num, int fps_den, bool df)
: mFramesSinceMidnight(frames_since_midnight)
{
    //fprintf(stderr, "frames_since_midnight %d, fps_num %d, fps_den %d, %s\n", frames_since_midnight, fps_num, fps_den, df ? "DF" : "NDF");

    mFormat = GetFormat(fps_num, fps_den, df);
    CheckFramesSinceMidnight();
    UpdateHoursMinsEtc();
    UpdateText();
}

// constructor from frame count and format enum
Timecode::Timecode(int frames_since_midnight, FormatEnumType format)
: mFormat(format), mFramesSinceMidnight(frames_since_midnight)
{
    CheckFramesSinceMidnight();
    UpdateHoursMinsEtc();
    UpdateText();
}

// constructor from hrs, mins, secs, frames; and rate parameters
Timecode::Timecode(int hr, int min, int sec, int frame, int frame_of_pair, int fps_num, int fps_den, bool df)
: mHours(hr), mMinutes(min), mSeconds(sec), mFrames(frame), mFrameOfPair(frame_of_pair)
{
    mFormat = GetFormat(fps_num, fps_den, df);

    UpdateFramesSinceMidnight();
    UpdateText();
}

// constructor from hrs, mins, secs, frames; and format enum
Timecode::Timecode(int hr, int min, int sec, int frame, int frame_of_pair, FormatEnumType format)
: mFormat(format), mHours(hr), mMinutes(min), mSeconds(sec), mFrames(frame), mFrameOfPair(frame_of_pair)
{
    UpdateFramesSinceMidnight();
    UpdateText();
}

// constructor from text; and rate parameters
Timecode::Timecode(const char * s, int fps_num, int fps_den, bool df)
{
    mFormat = GetFormat(fps_num, fps_den, df);

    bool ok = false;
    if (11 == strlen(s))
    {
        if (4 == sscanf(s,"%2d:%2d:%2d:%2d", &mHours, &mMinutes, &mSeconds, &mFrames))
        {
            ok = true;
        }
        else if (4 == sscanf(s,"%2d:%2d:%2d.%2d", &mHours, &mMinutes, &mSeconds, &mFrames))
        {
            //mDropFrame = true;
            ok = true;
        }
        else if (4 == sscanf(s,"%2d:%2d:%2d;%2d", &mHours, &mMinutes, &mSeconds, &mFrames))
        {
            //mDropFrame = true;
            ok = true;
        }
        mFrameOfPair = 0;
    }
    else if (8 == strlen(s))
    {
        if (3 == sscanf(s,"%2d:%2d:%2d", &mHours, &mMinutes, &mSeconds))
        {
            ok = true;
        }
        mFrames = 0;
        mFrameOfPair = 0;
    }

    if (!ok)
    {
        mFormat = NONE;
    }

    UpdateFramesSinceMidnight();
    UpdateText();
}

// constructor from text and format enum
Timecode::Timecode(const char * s, FormatEnumType format)
: mFormat(format)
{
    bool ok = false;
    if (11 == strlen(s))
    {
        if (4 == sscanf(s,"%2d:%2d:%2d:%2d", &mHours, &mMinutes, &mSeconds, &mFrames))
        {
            ok = true;
        }
        else if (4 == sscanf(s,"%2d:%2d:%2d.%2d", &mHours, &mMinutes, &mSeconds, &mFrames))
        {
            //mDropFrame = true;
            ok = true;
        }
        else if (4 == sscanf(s,"%2d:%2d:%2d;%2d", &mHours, &mMinutes, &mSeconds, &mFrames))
        {
            //mDropFrame = true;
            ok = true;
        }
    }
    else if (8 == strlen(s))
    {
        if (3 == sscanf(s,"%2d:%2d:%2d", &mHours, &mMinutes, &mSeconds))
        {
            mFrames = 0;
            ok = true;
        }
    }

    if (!ok)
    {
        mFormat = NONE;
    }

    UpdateFramesSinceMidnight();
    UpdateText();
}

// Copy constructor
Timecode::Timecode(const Timecode & tc)
{
    mFormat = tc.mFormat;
    mFramesSinceMidnight = tc.mFramesSinceMidnight;
    mHours = tc.mHours;
    mMinutes = tc.mMinutes;
    mSeconds = tc.mSeconds;
    mFrames = tc.mFrames;
    mFrameOfPair = tc.mFrameOfPair;

    UpdateText();
}

Timecode & Timecode::operator=(const Timecode & tc)
{
    if (this != &tc)
    {
        mFormat = tc.mFormat;
        mFramesSinceMidnight = tc.mFramesSinceMidnight;
        mHours = tc.mHours;
        mMinutes = tc.mMinutes;
        mSeconds = tc.mSeconds;
        mFrames = tc.mFrames;
        mFrameOfPair = tc.mFrameOfPair;
        UpdateText();
    }
    return *this;
}

/*
Timecode & Timecode::operator=(int frames_since_midnight)
{
    // Guard against garbage data
    if (frames_since_midnight < 0)
    {
        mIsNull = true;
    }
    else
    {
        mFramesSinceMidnight = frames_since_midnight;
        mIsNull = false;
    }
    UpdateHoursMinsEtc();
    UpdateText();
    return *this;
}
*/

const char * Timecode::FormatName() const
{
    switch (mFormat)
    {
    case TC_25:
        return "TC_25";
        break;
    case TC_50:
        return "TC_50";
        break;
    case TC_29DF:
        return "TC_29DF";
        break;
    case TC_29NDF:
        return "TC_29NDF";
        break;
    case TC_59DF:
        return "TC_59DF";
        break;
    case TC_59NDF:
        return "TC_59NDF";
        break;
    case NONE:
        return "";
        break;
    }

    return "";
}

void Timecode::UpdateHoursMinsEtc()
{
    int frames = mFramesSinceMidnight;
    int nominal_fps = NominalFps();

    int ten_minute;  // used in drop-frame calculation
    int unit_minute; // used in drop-frame calculation

    switch (mFormat)
    {
    case TC_25:
    case TC_29NDF:
        mHours = frames / (nominal_fps * 60 * 60);
        frames %= (nominal_fps * 60 * 60);
        mMinutes = frames / (nominal_fps * 60);
        frames %= (nominal_fps * 60);
        mSeconds = frames / nominal_fps;
        frames %= nominal_fps;
        mFrames = frames;
        mFrameOfPair = 0;
        break;
    case TC_50:
    case TC_59NDF:
        mHours = frames / (nominal_fps * 60 * 60);
        frames %= (nominal_fps * 60 * 60);
        mMinutes = frames / (nominal_fps * 60);
        frames %= (nominal_fps * 60);
        mSeconds = frames / nominal_fps;
        frames %= nominal_fps;
        mFrames = frames / 2;
        frames %= 2;
        mFrameOfPair = frames;
        break;
    case TC_29DF:
        mHours = frames / DF_FRAMES_PER_HOUR;
        frames %= DF_FRAMES_PER_HOUR;

        ten_minute = frames / DF_FRAMES_PER_TEN_MINUTE;
        frames %= DF_FRAMES_PER_TEN_MINUTE;

        // must adjust frame count to make minutes calculation work
        // calculations from here on in just assume that within the 10 minute cycle
        // there are only DF_FRAMES_PER_MINUTE (1798) frames per minute - even for the first minute
        // in the ten minute cycle. Hence we decrement the frame count by 2 to get the minutes count
        // So for the first two frames of the ten minute cycle we get a negative frames number

        frames -= 2;

        if (frames < 0)
        {
            unit_minute = 0;
        }
        else
        {
            unit_minute = frames / DF_FRAMES_PER_MINUTE;
            frames %= DF_FRAMES_PER_MINUTE;
        }
        mMinutes = ten_minute * 10 + unit_minute;

        // frames now contains frame in minute @ 1798 frames per minute
        // put the 2 frame adjustment back in to get the correct frame count
        // For the first two frames of the ten minute cycle, frames is negative and this
        // adjustment makes it non-negative. For other minutes in the cycle the frames count
        // goes from 0 upwards, thus this adjusment gives the required 2 frame offset

        frames += 2;

        mSeconds = frames / 30;
        mFrames = frames % 30;
        mFrameOfPair = 0;
        break;
    case TC_59DF:
        // as above but we halve the frames since midnight and
        // do calculation as if rate were halved, i.e. use same
        // constants DF_FRAMES_PER_HOUR etc. as for 29.97 rate
        mFrameOfPair = frames % 2;
        frames /= 2;

        mHours = frames / DF_FRAMES_PER_HOUR;
        frames %= DF_FRAMES_PER_HOUR;

        ten_minute = frames / DF_FRAMES_PER_TEN_MINUTE;
        frames %= DF_FRAMES_PER_TEN_MINUTE;

        // must adjust frame count to make minutes calculation work
        // calculations from here on in just assume that within the 10 minute cycle
        // there are only DF_FRAMES_PER_MINUTE (1798) frames per minute - even for the first minute
        // in the ten minute cycle. Hence we decrement the frame count by 2 to get the minutes count
        // So for the first two frames of the ten minute cycle we get a negative frames number

        frames -= 2;

        unit_minute = frames / DF_FRAMES_PER_MINUTE;
        frames %= DF_FRAMES_PER_MINUTE;
        mMinutes = ten_minute * 10 + unit_minute;

        // frames now contains frame in minute @ 1798 frames per minute
        // put the 2 frame adjustment back in to get the correct frame count
        // For the first two frames of the ten minute cycle, frames is negative and this
        // adjustment makes it non-negative. For other minutes in the cycle the frames count
        // goes from 0 upwards, thus this adjusment gives the required 2 frame offset

        frames += 2;

        mSeconds = frames / 30;
        mFrames = frames % 30;
        break;
    case NONE:
        // do nothing
        break;
    }

    // Prevent hours exceeding one day
    mHours %= 24;
}

void Timecode::UpdateFramesSinceMidnight()
{
    int nominal_fps = NominalFps();

    switch (mFormat)
    {
    case TC_25:
    case TC_29NDF:
        mFramesSinceMidnight = mFrames
            + mSeconds * nominal_fps
            + mMinutes * 60 * nominal_fps
            + mHours * 60 * 60 * nominal_fps;
        break;
    case TC_50:
    case TC_59NDF:
        mFramesSinceMidnight = mFrameOfPair
            + mFrames * 2
            + mSeconds * nominal_fps
            + mMinutes * 60 * nominal_fps
            + mHours * 60 * 60 * nominal_fps;
        break;
    case TC_29DF:
        mFramesSinceMidnight = mFrames
            + mSeconds * 30
            + (mMinutes % 10) * DF_FRAMES_PER_MINUTE
            + (mMinutes / 10) * DF_FRAMES_PER_TEN_MINUTE
            + mHours * DF_FRAMES_PER_HOUR;
        /* alternative code
        mFramesSinceMidnight = mFrames
            + mSeconds * 30
            + mMinutes * DF_FRAMES_PER_MINUTE
            + (mMinutes / 10) * 2
            + mHours * DF_FRAMES_PER_HOUR;
        */
        break;
    case TC_59DF:
        mFramesSinceMidnight = mFrames
            + mSeconds * 30
            + (mMinutes % 10) * DF_FRAMES_PER_MINUTE
            + (mMinutes / 10) * DF_FRAMES_PER_TEN_MINUTE
            + mHours * DF_FRAMES_PER_HOUR;
        mFramesSinceMidnight *= 2;
        mFramesSinceMidnight += mFrameOfPair;
        break;
    case NONE:
        break;
    }
}

/**
This method is used to update text representation of timecode whenever numerical
values change.
Unfortunately you can't produce the text in Text() function because it would not
then be const.
*/
void Timecode::UpdateText()
{
    switch (mFormat)
    {
    case NONE:
        sprintf(mText, "??:??:??:??");
        sprintf(mTextNoSeparators, "????????");
        break;
    case TC_25:
    case TC_50:
    case TC_29NDF:
    case TC_59NDF:
        if (0 == mFrameOfPair)
        {
            // with colon
            sprintf(mText,"%02d:%02d:%02d:%02d", mHours, mMinutes, mSeconds, mFrames);
            sprintf(mTextNoSeparators,"%02d%02d%02d%02d", mHours, mMinutes, mSeconds, mFrames);
        }
        else
        {
            // with full-stop
            sprintf(mText,"%02d:%02d:%02d.%02d", mHours, mMinutes, mSeconds, mFrames);
            sprintf(mTextNoSeparators,"%02d%02d%02d%02d", mHours, mMinutes, mSeconds, mFrames);
        }
        break;
    case TC_29DF:
    case TC_59DF:
        if (0 == mFrameOfPair)
        {
            // with semi-colon
            sprintf(mText,"%02d:%02d:%02d;%02d", mHours, mMinutes, mSeconds, mFrames);
            sprintf(mTextNoSeparators,"%02d%02d%02d%02d", mHours, mMinutes, mSeconds, mFrames);
        }
        else
        {
            // with comma
            sprintf(mText,"%02d:%02d:%02d,%02d", mHours, mMinutes, mSeconds, mFrames);
            sprintf(mTextNoSeparators,"%02d%02d%02d%02d", mHours, mMinutes, mSeconds, mFrames);
        }
        break;
    }
}

Timecode::FormatEnumType Timecode::GetFormat(int fps_num, int fps_den, bool df)
{
    Timecode::FormatEnumType fmt = Timecode::NONE;

    if (fps_num == 25 && fps_den == 1)
    {
        fmt = TC_25;
    }
    if (fps_num == 50 && fps_den == 1)
    {
        fmt = TC_50;
    }
    else if (fps_num == 30000 && fps_den == 1001)
    {
        if (df)
        {
            fmt = TC_29DF;
        }
        else
        {
            fmt = TC_29NDF;
        }
    }
    else if (fps_num == 60000 && fps_den == 1001)
    {
        if (df)
        {
            fmt = TC_59DF;
        }
        else
        {
            fmt = TC_59NDF;
        }
    }


    return fmt;
}

int Timecode::FrameRateNumerator() const
{
    int fps_num;

    switch (mFormat)
    {
    case TC_25:
        fps_num = 25;
        break;
    case TC_50:
        fps_num = 50;
        break;
    case TC_29DF:
    case TC_29NDF:
        fps_num = 30000;
        break;
    case TC_59DF:
    case TC_59NDF:
        fps_num = 60000;
        break;
    default:
        fps_num = 0;
        break;
    }

    return fps_num;
}

int Timecode::FrameRateDenominator() const
{
    int fps_den = 0;

    switch (mFormat)
    {
    case TC_25:
    case TC_50:
        fps_den = 1;
        break;
    case TC_29DF:
    case TC_29NDF:
    case TC_59DF:
    case TC_59NDF:
        fps_den = 1001;
        break;
    case NONE:
        fps_den = 0;
        break;
    }

    return fps_den;
}

int Timecode::NominalFps()
{
    int fps = 0;

    switch (mFormat)
    {
    case TC_25:
        fps = 25;
        break;
    case TC_50:
        fps = 50;
        break;
    case TC_29DF:
    case TC_29NDF:
        fps = 30;
        break;
    case TC_59DF:
    case TC_59NDF:
        fps = 60;
        break;
    case NONE:
        fps = 0;
        break;
    }

    return fps;
}

bool Timecode::DropFrame() const
{
    bool df = false;

    switch (mFormat)
    {
    case TC_25:
    case TC_50:
    case TC_29NDF:
    case TC_59NDF:
    case NONE:
        df = false;
        break;
    case TC_29DF:
    case TC_59DF:
        df = true;
        break;
    }

    return df;
}

/**
Check and adjust mFramesSinceMidnight to keep within
allowed range.
*/
void Timecode::CheckFramesSinceMidnight()
{
    if (!IsNull())
    {
        // Not quite right for drop-frame formats because
        // the number of frames-per-day is non-integer.
        // In non-drop 1000/1001 rates we are fine because
        // the "day" length is adjusted.
        int frames_per_day = NominalFps() * 24 * 60 * 60;
        if (DropFrame())
        {
            frames_per_day -= frames_per_day / 1001;
        }

        if (mFramesSinceMidnight < 0)
        {
            mFramesSinceMidnight += frames_per_day;
        }
        mFramesSinceMidnight %= frames_per_day;
    }
}

/**
Add a number of frames.
*/
void Timecode::operator+=(int frames)
{
    if (!IsNull())
    {
        mFramesSinceMidnight += frames;
        UpdateHoursMinsEtc();
        UpdateText();
    }
}

/**
Subtract a number of frames.
*/
void Timecode::operator-=(int frames)
{
    if (!IsNull())
    {
        mFramesSinceMidnight -= frames;
        UpdateHoursMinsEtc();
        UpdateText();
    }
}



Duration::Duration(int frames, int fps_num, int fps_den, bool df)
: mFrameCount(frames), mFrameRateNumerator(fps_num), mFrameRateDenominator(fps_den), mDropFrame(df)
{
    UpdateHoursMinsEtc();
    UpdateText();
}

void Duration::UpdateHoursMinsEtc()
{
    int frames = mFrameCount;
    int nominal_fps = mFrameRateNumerator / mFrameRateDenominator
        + (mFrameRateNumerator % mFrameRateDenominator ? 1 : 0);

    if (!mDropFrame)
    {
        mHours = frames / (nominal_fps * 60 * 60);
        frames %= (nominal_fps * 60 * 60);
        mMinutes = frames / (nominal_fps * 60);
        frames %= (nominal_fps * 60);
        mSeconds = frames / nominal_fps;
        mFrames = frames % nominal_fps;
    }
    else
    {
        mHours = frames / DF_FRAMES_PER_HOUR;
        frames %= DF_FRAMES_PER_HOUR;

        int ten_minute = frames / DF_FRAMES_PER_TEN_MINUTE;
        frames %= DF_FRAMES_PER_TEN_MINUTE;

        // must adjust frame count to make minutes calculation work
        // calculations from here on in just assume that within the 10 minute cycle
        // there are only DF_FRAMES_PER_MINUTE (1798) frames per minute - even for the first minute
        // in the ten minute cycle. Hence we decrement the frame count by 2 to get the minutes count
        // So for the first two frames of the ten minute cycle we get a negative frames number

        frames -= 2;

        int unit_minute = frames / DF_FRAMES_PER_MINUTE;
        frames %= DF_FRAMES_PER_MINUTE;
        mMinutes = ten_minute * 10 + unit_minute;

        // frames now contains frame in minute @ 1798 frames per minute
        // put the 2 frame adjustment back in to get the correct frame count
        // For the first two frames of the ten minute cycle, frames is negative and this
        // adjustment makes it non-negative. For other minutes in the cycle the frames count
        // goes from 0 upwards, thus this adjusment gives the required 2 frame offset

        frames += 2;

        mSeconds = frames / nominal_fps;
        mFrames = frames % nominal_fps;
    }
}

/**
This method is used to update text representation of duration whenever numerical
values change.
*/
void Duration::UpdateText()
{
#if 1
    // Full string including frames
    if (mDropFrame)
    {
        sprintf(mText,"%02d:%02d:%02d;%02d", mHours, mMinutes, mSeconds, mFrames);
    }
    else
    {
        sprintf(mText,"%02d:%02d:%02d:%02d", mHours, mMinutes, mSeconds, mFrames);
    }
#else
    // Shorter version without frames
    if (mHours == 0)
    {
        sprintf(mText,"%02d:%02d", mMinutes, mSeconds);
    }
    else
    {
        sprintf(mText,"%d:%02d:%02d", mHours, mMinutes, mSeconds);
    }
#endif
}

namespace Ingex
{

/**
Get difference between two Timecodes.
Duration operator-(const Timecode & lhs, const Timecode & rhs)
{
    // NB. Should check for mode compatibility
    return Duration(lhs.FramesSinceMidnight() - rhs.FramesSinceMidnight(), lhs.mFrameRateNumerator, lhs.mFrameRateDenominator, lhs.mDropFrame);
}
*/

/**
Add a number of frames to a timecode.
*/
Timecode operator+(const Timecode & lhs, const int & frames)
{
    return Timecode(lhs.mFramesSinceMidnight + frames, lhs.mFormat);
}

} // namespace



