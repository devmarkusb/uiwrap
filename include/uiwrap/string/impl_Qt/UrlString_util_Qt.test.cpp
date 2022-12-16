#include "UrlString_util_Qt.h"
#include "StringConvert_Qt.h"
#include "gtest/gtest.h"

#include "ul/macros.h"


TEST(UrlString_util_Qt, remove)
{
#if UL_WINDOWS
    QString res{uiw::implQt::removeFile_urlSchemePrefix(QStringLiteral("file:///c:/temp/xy/z.txt"))};
    EXPECT_STREQ("c:/temp/xy/z.txt", uiw::implQt::qs2s(res).c_str());
#else
    QString res{uiw::implQt::removeFile_urlSchemePrefix(QStringLiteral("file:///home/xy/temp/xy/z.txt"))};
    EXPECT_STREQ("/home/xy/temp/xy/z.txt", uiw::implQt::qs2s(res).c_str());
#endif
}

TEST(UrlString_util_Qt, prepend)
{
    QString res{uiw::implQt::prependFile_urlSchemePrefix(QStringLiteral("c:/temp/xy/z.txt"))};
    EXPECT_STREQ("file:///c:/temp/xy/z.txt", uiw::implQt::qs2s(res).c_str());
}
