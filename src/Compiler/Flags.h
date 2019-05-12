/*
 * Flags.h
 * 
 * This file is part of the XShaderCompiler project (Copyright (c) 2014-2019 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef XSC_FLAGS_H
#define XSC_FLAGS_H


namespace Xsc
{


// Common flags class.
class Flags
{

    public:

        Flags() = default;
        Flags(const Flags&) = default;
        Flags& operator = (const Flags&) = default;

        // Initializes the object with the specified flags.
        inline Flags(unsigned int flags) :
            bitMask_ { flags }
        {
        }

        // Sets the specified flag only once and returns true if the flag has not already been set.
        inline bool SetOnce(unsigned int flag)
        {
            if (!All(flag))
            {
                *this << flag;
                return true;
            }
            return false;
        }

        // Sets the specified flag.
        inline void Insert(unsigned int flag)
        {
            bitMask_ |= flag;
        }

        // Removes the specified flag.
        inline void Remove(unsigned int flag)
        {
            bitMask_ &= (~flag);
        }

        // Sets the specified flag (see 'Insert' function).
        inline Flags& operator << (unsigned int flag)
        {
            Insert(flag);
            return *this;
        }

        // Returns true if this flags instance has any of the specified flags.
        inline bool Any(unsigned int flags) const
        {
            return ((bitMask_ & flags) != 0);
        }

        // Returns true if this flags instance has all of the specified flags.
        inline bool All(unsigned int flags) const
        {
            return ((bitMask_ & flags) == flags);
        }

        // Returns true if the specified flag is set.
        inline bool operator () (unsigned int flag) const
        {
            return Any(flag);
        }

        // Returns the flags bit mask.
        inline operator unsigned int () const
        {
            return bitMask_;
        }

    private:

        unsigned int bitMask_ = 0;

};


} // /namespace Xsc


#endif



// ================================================================================