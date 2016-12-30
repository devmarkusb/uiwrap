#include "UrlString_util_Qt.h"
#include "StringConvert_Qt.h"
#include "gtest/gtest.h"


TEST(UrlString_util_Qt, remove)
{
    QString res{uiw::implQt::removeFile_urlSchemePrefix(QStringLiteral("file:///c:/temp/xy/z.txt"))};
    EXPECT_STREQ("c:/temp/xy/z.txt", uiw::implQt::qs2s(res).c_str());
}

TEST(UrlString_util_Qt, prepend)
{
    QString res{uiw::implQt::prependFile_urlSchemePrefix(QStringLiteral("c:/temp/xy/z.txt"))};
    EXPECT_STREQ("file:///c:/temp/xy/z.txt", uiw::implQt::qs2s(res).c_str());
}
