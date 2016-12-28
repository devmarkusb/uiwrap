// Markus Borris, 2016
// This file is part of my uiwrap library.

//!
/** Important notice:
    I decided to make any conversion safe/checked where appropriate and possible.
    That is, range violations result in exceptions. The exceptions are of the
    same type as the ones thrown by assertions from Toolib/assert.h.
    The reason is that these functions ought to be called right at the interface
    between GUI and backend only. And this interface is thought to be *the* crucial
    point of error checking. User input has to be checked as thoroughly as possible.
    Below that, in the backend, of course we wozuld only check for programmers
    errors, which we don't.
*/
//! \file

#ifndef CONVERT_QT_H_djkghuinx478gn4289gf32
#define CONVERT_QT_H_djkghuinx478gn4289gf32
//#ifdef UIW_LINKLIB_IMPL_CHOICE_QT

#include "Toolib/narrow.h"
#include "Toolib/std/std_extensions.h"
#include "Toolib/PPDefs/MSVC/SUPPRESS_WARNINGS_EXTERNAL_BEGIN"
#include <QtGlobal>
#include "Toolib/PPDefs/MSVC/SUPPRESS_WARNINGS_EXTERNAL_END"
#include <type_traits>


namespace uiw
{
namespace implQt
{
//####################################################################################################################
//! From qreal.
/** Be careful when converting qreal to float and qreal is double in your case.
    You should investigate the difference of the two w.r.t. double and decide whether
    the loss of precision or even range is acceptable for you.*/
template <typename TargetType>
// TargetType expected to be floating point.
constexpr
typename too::enable_if_t<std::is_same<qreal, float>::value, TargetType>
from_qreal(qreal v) noexcept
{
    static_assert(std::is_floating_point<TargetType>::value, "target type needs to be float, double or long double");
    return v;
}

template <typename TargetType>
// TargetType expected to be floating point.
constexpr
typename too::enable_if_t<std::is_same<qreal, double>::value, TargetType>
from_qreal(qreal v) noexcept
{
    static_assert(std::is_floating_point<TargetType>::value, "target type needs to be float, double or long double");
    return v;
}

template <>
inline constexpr
typename too::enable_if_t<std::is_same<qreal, double>::value, float>
from_qreal<float>(qreal v) noexcept
{
    return too::narrow_cast<float>(v);
}


//####################################################################################################################
//! To qreal.
/** Be careful when converting long double to qreal (or even double in case you qreal is float),
    You should investigate the difference of the two w.r.t. the larger type and decide whether
    the loss of precision or even range is acceptable for you.*/
template <typename SourceType>
// SourceType expected to be floating point.
constexpr
qreal to_qreal(typename too::enable_if_t<std::is_same<qreal, float>::value, SourceType> v) noexcept
{
    static_assert(std::is_floating_point<SourceType>::value, "source type needs to be float, double or long double");
    return too::narrow_cast<qreal>(v);
}

template <typename SourceType>
// SourceType expected to be floating point.
constexpr
typename too::enable_if_t<std::is_same<qreal, double>::value, qreal>
to_qreal(SourceType v) noexcept
{
    static_assert(std::is_floating_point<SourceType>::value, "source type needs to be float, double or long double");
    return v;
}

template <>
inline constexpr
typename too::enable_if_t<std::is_same<qreal, double>::value, qreal>
to_qreal<long double>(long double v) noexcept
{
    return too::narrow_cast<qreal>(v);
}

//####################################################################################################################
} // implQt
} // uiw

//#endif //UIW_LINKLIB_IMPL_CHOICE_QT
#endif
