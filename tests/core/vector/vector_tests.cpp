#include "gtest/gtest.h"

import core.vector;

using namespace testing;
using namespace gg;

void compare_vec4(const fvec4& vec, float x, float y, float z, float w)
{
    EXPECT_FLOAT_EQ(vec.x, x);
    EXPECT_FLOAT_EQ(vec.y, y);
    EXPECT_FLOAT_EQ(vec.z, z);
    EXPECT_FLOAT_EQ(vec.w, w);
}

void compare_all_vec4_x_compinations(const fvec4& vec, float x, float y, float z, float w)
{
    compare_vec4(vec.xxxx, x, x, x, x);
    compare_vec4(vec.xxxy, x, x, x, y);
    compare_vec4(vec.xxxz, x, x, x, z);
    compare_vec4(vec.xxxw, x, x, x, w);

    compare_vec4(vec.xxyx, x, x, y, x);
    compare_vec4(vec.xxyy, x, x, y, y);
    compare_vec4(vec.xxyz, x, x, y, z);
    compare_vec4(vec.xxyw, x, x, y, w);

    compare_vec4(vec.xxzx, x, x, z, x);
    compare_vec4(vec.xxzy, x, x, z, y);
    compare_vec4(vec.xxzz, x, x, z, z);
    compare_vec4(vec.xxzw, x, x, z, w);

    compare_vec4(vec.xxwx, x, x, w, x);
    compare_vec4(vec.xxwy, x, x, w, y);
    compare_vec4(vec.xxwz, x, x, w, z);
    compare_vec4(vec.xxww, x, x, w, w);

    compare_vec4(vec.xyxx, x, y, x, x);
    compare_vec4(vec.xyxy, x, y, x, y);
    compare_vec4(vec.xyxz, x, y, x, z);
    compare_vec4(vec.xyxw, x, y, x, w);

    compare_vec4(vec.xyyx, x, y, y, x);
    compare_vec4(vec.xyyy, x, y, y, y);
    compare_vec4(vec.xyyz, x, y, y, z);
    compare_vec4(vec.xyyw, x, y, y, w);

    compare_vec4(vec.xyzx, x, y, z, x);
    compare_vec4(vec.xyzy, x, y, z, y);
    compare_vec4(vec.xyzz, x, y, z, z);
    compare_vec4(vec.xyzw, x, y, z, w);

    compare_vec4(vec.xywx, x, y, w, x);
    compare_vec4(vec.xywy, x, y, w, y);
    compare_vec4(vec.xywz, x, y, w, z);
    compare_vec4(vec.xyww, x, y, w, w);

    compare_vec4(vec.xzxx, x, z, x, x);
    compare_vec4(vec.xzxy, x, z, x, y);
    compare_vec4(vec.xzxz, x, z, x, z);
    compare_vec4(vec.xzxw, x, z, x, w);

    compare_vec4(vec.xzyx, x, z, y, x);
    compare_vec4(vec.xzyy, x, z, y, y);
    compare_vec4(vec.xzyz, x, z, y, z);
    compare_vec4(vec.xzyw, x, z, y, w);

    compare_vec4(vec.xzzx, x, z, z, x);
    compare_vec4(vec.xzzy, x, z, z, y);
    compare_vec4(vec.xzzz, x, z, z, z);
    compare_vec4(vec.xzzw, x, z, z, w);

    compare_vec4(vec.xzwx, x, z, w, x);
    compare_vec4(vec.xzwy, x, z, w, y);
    compare_vec4(vec.xzwz, x, z, w, z);
    compare_vec4(vec.xzww, x, z, w, w);

    compare_vec4(vec.xwxx, x, w, x, x);
    compare_vec4(vec.xwxy, x, w, x, y);
    compare_vec4(vec.xwxz, x, w, x, z);
    compare_vec4(vec.xwxw, x, w, x, w);

    compare_vec4(vec.xwyx, x, w, y, x);
    compare_vec4(vec.xwyy, x, w, y, y);
    compare_vec4(vec.xwyz, x, w, y, z);
    compare_vec4(vec.xwyw, x, w, y, w);

    compare_vec4(vec.xwzx, x, w, z, x);
    compare_vec4(vec.xwzy, x, w, z, y);
    compare_vec4(vec.xwzz, x, w, z, z);
    compare_vec4(vec.xwzw, x, w, z, w);

    compare_vec4(vec.xwwx, x, w, w, x);
    compare_vec4(vec.xwwy, x, w, w, y);
    compare_vec4(vec.xwwz, x, w, w, z);
    compare_vec4(vec.xwww, x, w, w, w);
}

void compare_all_vec4_y_compinations(const fvec4& vec, float x, float y, float z, float w)
{
    compare_vec4(vec.yxxx, y, x, x, x);
    compare_vec4(vec.yxxy, y, x, x, y);
    compare_vec4(vec.yxxz, y, x, x, z);
    compare_vec4(vec.yxxw, y, x, x, w);

    compare_vec4(vec.yxyx, y, x, y, x);
    compare_vec4(vec.yxyy, y, x, y, y);
    compare_vec4(vec.yxyz, y, x, y, z);
    compare_vec4(vec.yxyw, y, x, y, w);

    compare_vec4(vec.yxzx, y, x, z, x);
    compare_vec4(vec.yxzy, y, x, z, y);
    compare_vec4(vec.yxzz, y, x, z, z);
    compare_vec4(vec.yxzw, y, x, z, w);

    compare_vec4(vec.yxwx, y, x, w, x);
    compare_vec4(vec.yxwy, y, x, w, y);
    compare_vec4(vec.yxwz, y, x, w, z);
    compare_vec4(vec.yxww, y, x, w, w);

    compare_vec4(vec.yyxx, y, y, x, x);
    compare_vec4(vec.yyxy, y, y, x, y);
    compare_vec4(vec.yyxz, y, y, x, z);
    compare_vec4(vec.yyxw, y, y, x, w);

    compare_vec4(vec.yyyx, y, y, y, x);
    compare_vec4(vec.yyyy, y, y, y, y);
    compare_vec4(vec.yyyz, y, y, y, z);
    compare_vec4(vec.yyyw, y, y, y, w);

    compare_vec4(vec.yyzx, y, y, z, x);
    compare_vec4(vec.yyzy, y, y, z, y);
    compare_vec4(vec.yyzz, y, y, z, z);
    compare_vec4(vec.yyzw, y, y, z, w);

    compare_vec4(vec.yywx, y, y, w, x);
    compare_vec4(vec.yywy, y, y, w, y);
    compare_vec4(vec.yywz, y, y, w, z);
    compare_vec4(vec.yyww, y, y, w, w);

    compare_vec4(vec.yzxx, y, z, x, x);
    compare_vec4(vec.yzxy, y, z, x, y);
    compare_vec4(vec.yzxz, y, z, x, z);
    compare_vec4(vec.yzxw, y, z, x, w);

    compare_vec4(vec.yzyx, y, z, y, x);
    compare_vec4(vec.yzyy, y, z, y, y);
    compare_vec4(vec.yzyz, y, z, y, z);
    compare_vec4(vec.yzyw, y, z, y, w);

    compare_vec4(vec.yzzx, y, z, z, x);
    compare_vec4(vec.yzzy, y, z, z, y);
    compare_vec4(vec.yzzz, y, z, z, z);
    compare_vec4(vec.yzzw, y, z, z, w);

    compare_vec4(vec.yzwx, y, z, w, x);
    compare_vec4(vec.yzwy, y, z, w, y);
    compare_vec4(vec.yzwz, y, z, w, z);
    compare_vec4(vec.yzww, y, z, w, w);

    compare_vec4(vec.ywxx, y, w, x, x);
    compare_vec4(vec.ywxy, y, w, x, y);
    compare_vec4(vec.ywxz, y, w, x, z);
    compare_vec4(vec.ywxw, y, w, x, w);

    compare_vec4(vec.ywyx, y, w, y, x);
    compare_vec4(vec.ywyy, y, w, y, y);
    compare_vec4(vec.ywyz, y, w, y, z);
    compare_vec4(vec.ywyw, y, w, y, w);

    compare_vec4(vec.ywzx, y, w, z, x);
    compare_vec4(vec.ywzy, y, w, z, y);
    compare_vec4(vec.ywzz, y, w, z, z);
    compare_vec4(vec.ywzw, y, w, z, w);

    compare_vec4(vec.ywwx, y, w, w, x);
    compare_vec4(vec.ywwy, y, w, w, y);
    compare_vec4(vec.ywwz, y, w, w, z);
    compare_vec4(vec.ywww, y, w, w, w);
}

void compare_all_vec4_z_compinations(const fvec4& vec, float x, float y, float z, float w)
{
    compare_vec4(vec.zxxx, z, x, x, x);
    compare_vec4(vec.zxxy, z, x, x, y);
    compare_vec4(vec.zxxz, z, x, x, z);
    compare_vec4(vec.zxxw, z, x, x, w);

    compare_vec4(vec.zxyx, z, x, y, x);
    compare_vec4(vec.zxyy, z, x, y, y);
    compare_vec4(vec.zxyz, z, x, y, z);
    compare_vec4(vec.zxyw, z, x, y, w);

    compare_vec4(vec.zxzx, z, x, z, x);
    compare_vec4(vec.zxzy, z, x, z, y);
    compare_vec4(vec.zxzz, z, x, z, z);
    compare_vec4(vec.zxzw, z, x, z, w);

    compare_vec4(vec.zxwx, z, x, w, x);
    compare_vec4(vec.zxwy, z, x, w, y);
    compare_vec4(vec.zxwz, z, x, w, z);
    compare_vec4(vec.zxww, z, x, w, w);

    compare_vec4(vec.zyxx, z, y, x, x);
    compare_vec4(vec.zyxy, z, y, x, y);
    compare_vec4(vec.zyxz, z, y, x, z);
    compare_vec4(vec.zyxw, z, y, x, w);

    compare_vec4(vec.zyyx, z, y, y, x);
    compare_vec4(vec.zyyy, z, y, y, y);
    compare_vec4(vec.zyyz, z, y, y, z);
    compare_vec4(vec.zyyw, z, y, y, w);

    compare_vec4(vec.zyzx, z, y, z, x);
    compare_vec4(vec.zyzy, z, y, z, y);
    compare_vec4(vec.zyzz, z, y, z, z);
    compare_vec4(vec.zyzw, z, y, z, w);

    compare_vec4(vec.zywx, z, y, w, x);
    compare_vec4(vec.zywy, z, y, w, y);
    compare_vec4(vec.zywz, z, y, w, z);
    compare_vec4(vec.zyww, z, y, w, w);

    compare_vec4(vec.zzxx, z, z, x, x);
    compare_vec4(vec.zzxy, z, z, x, y);
    compare_vec4(vec.zzxz, z, z, x, z);
    compare_vec4(vec.zzxw, z, z, x, w);

    compare_vec4(vec.zzyx, z, z, y, x);
    compare_vec4(vec.zzyy, z, z, y, y);
    compare_vec4(vec.zzyz, z, z, y, z);
    compare_vec4(vec.zzyw, z, z, y, w);

    compare_vec4(vec.zzzx, z, z, z, x);
    compare_vec4(vec.zzzy, z, z, z, y);
    compare_vec4(vec.zzzz, z, z, z, z);
    compare_vec4(vec.zzzw, z, z, z, w);

    compare_vec4(vec.zzwx, z, z, w, x);
    compare_vec4(vec.zzwy, z, z, w, y);
    compare_vec4(vec.zzwz, z, z, w, z);
    compare_vec4(vec.zzww, z, z, w, w);

    compare_vec4(vec.zwxx, z, w, x, x);
    compare_vec4(vec.zwxy, z, w, x, y);
    compare_vec4(vec.zwxz, z, w, x, z);
    compare_vec4(vec.zwxw, z, w, x, w);

    compare_vec4(vec.zwyx, z, w, y, x);
    compare_vec4(vec.zwyy, z, w, y, y);
    compare_vec4(vec.zwyz, z, w, y, z);
    compare_vec4(vec.zwyw, z, w, y, w);

    compare_vec4(vec.zwzx, z, w, z, x);
    compare_vec4(vec.zwzy, z, w, z, y);
    compare_vec4(vec.zwzz, z, w, z, z);
    compare_vec4(vec.zwzw, z, w, z, w);

    compare_vec4(vec.zwwx, z, w, w, x);
    compare_vec4(vec.zwwy, z, w, w, y);
    compare_vec4(vec.zwwz, z, w, w, z);
    compare_vec4(vec.zwww, z, w, w, w);
}

void compare_all_vec4_w_compinations(const fvec4& vec, float x, float y, float z, float w)
{
    compare_vec4(vec.wxxx, w, x, x, x);
    compare_vec4(vec.wxxy, w, x, x, y);
    compare_vec4(vec.wxxz, w, x, x, z);
    compare_vec4(vec.wxxw, w, x, x, w);

    compare_vec4(vec.wxyx, w, x, y, x);
    compare_vec4(vec.wxyy, w, x, y, y);
    compare_vec4(vec.wxyz, w, x, y, z);
    compare_vec4(vec.wxyw, w, x, y, w);

    compare_vec4(vec.wxzx, w, x, z, x);
    compare_vec4(vec.wxzy, w, x, z, y);
    compare_vec4(vec.wxzz, w, x, z, z);
    compare_vec4(vec.wxzw, w, x, z, w);

    compare_vec4(vec.wxwx, w, x, w, x);
    compare_vec4(vec.wxwy, w, x, w, y);
    compare_vec4(vec.wxwz, w, x, w, z);
    compare_vec4(vec.wxww, w, x, w, w);

    compare_vec4(vec.wyxx, w, y, x, x);
    compare_vec4(vec.wyxy, w, y, x, y);
    compare_vec4(vec.wyxz, w, y, x, z);
    compare_vec4(vec.wyxw, w, y, x, w);

    compare_vec4(vec.wyyx, w, y, y, x);
    compare_vec4(vec.wyyy, w, y, y, y);
    compare_vec4(vec.wyyz, w, y, y, z);
    compare_vec4(vec.wyyw, w, y, y, w);

    compare_vec4(vec.wyzx, w, y, z, x);
    compare_vec4(vec.wyzy, w, y, z, y);
    compare_vec4(vec.wyzz, w, y, z, z);
    compare_vec4(vec.wyzw, w, y, z, w);

    compare_vec4(vec.wywx, w, y, w, x);
    compare_vec4(vec.wywy, w, y, w, y);
    compare_vec4(vec.wywz, w, y, w, z);
    compare_vec4(vec.wyww, w, y, w, w);

    compare_vec4(vec.wzxx, w, z, x, x);
    compare_vec4(vec.wzxy, w, z, x, y);
    compare_vec4(vec.wzxz, w, z, x, z);
    compare_vec4(vec.wzxw, w, z, x, w);

    compare_vec4(vec.wzyx, w, z, y, x);
    compare_vec4(vec.wzyy, w, z, y, y);
    compare_vec4(vec.wzyz, w, z, y, z);
    compare_vec4(vec.wzyw, w, z, y, w);

    compare_vec4(vec.wzzx, w, z, z, x);
    compare_vec4(vec.wzzy, w, z, z, y);
    compare_vec4(vec.wzzz, w, z, z, z);
    compare_vec4(vec.wzzw, w, z, z, w);

    compare_vec4(vec.wzwx, w, z, w, x);
    compare_vec4(vec.wzwy, w, z, w, y);
    compare_vec4(vec.wzwz, w, z, w, z);
    compare_vec4(vec.wzww, w, z, w, w);

    compare_vec4(vec.wwxx, w, w, x, x);
    compare_vec4(vec.wwxy, w, w, x, y);
    compare_vec4(vec.wwxz, w, w, x, z);
    compare_vec4(vec.wwxw, w, w, x, w);

    compare_vec4(vec.wwyx, w, w, y, x);
    compare_vec4(vec.wwyy, w, w, y, y);
    compare_vec4(vec.wwyz, w, w, y, z);
    compare_vec4(vec.wwyw, w, w, y, w);

    compare_vec4(vec.wwzx, w, w, z, x);
    compare_vec4(vec.wwzy, w, w, z, y);
    compare_vec4(vec.wwzz, w, w, z, z);
    compare_vec4(vec.wwzw, w, w, z, w);

    compare_vec4(vec.wwwx, w, w, w, x);
    compare_vec4(vec.wwwy, w, w, w, y);
    compare_vec4(vec.wwwz, w, w, w, z);
    compare_vec4(vec.wwww, w, w, w, w);
}


TEST(fvec2, TestAllCombinationsOfVec2)
{
    const float x_value = 0.0f;
    const float y_value = 1.0f;

    fvec2 test_vector{x_value, y_value};

    EXPECT_FLOAT_EQ(test_vector.x, x_value);
    EXPECT_FLOAT_EQ(test_vector.y, y_value);

    EXPECT_FLOAT_EQ(test_vector.xx[0], x_value);
    EXPECT_FLOAT_EQ(test_vector.xx[1], x_value);

    EXPECT_FLOAT_EQ(test_vector.xy[0], x_value);
    EXPECT_FLOAT_EQ(test_vector.xy[1], y_value);

    EXPECT_FLOAT_EQ(test_vector.yx[0], y_value);
    EXPECT_FLOAT_EQ(test_vector.yx[1], x_value);

    EXPECT_FLOAT_EQ(test_vector.yy[1], y_value);
    EXPECT_FLOAT_EQ(test_vector.yy[1], y_value);
}

TEST(fvec3, TestAllCombinationsOfVec3)
{
    const float x_value = 0.0f;
    const float y_value = 1.0f;
    const float z_value = 2.0f;

    fvec3 test_vector{x_value, y_value, z_value};

    EXPECT_FLOAT_EQ(test_vector.x, x_value);
    EXPECT_FLOAT_EQ(test_vector.y, y_value);
    EXPECT_FLOAT_EQ(test_vector.z, z_value);

    EXPECT_FLOAT_EQ(test_vector.xx[0], x_value);
    EXPECT_FLOAT_EQ(test_vector.xx[1], x_value);

    EXPECT_FLOAT_EQ(test_vector.xy[0], x_value);
    EXPECT_FLOAT_EQ(test_vector.xy[1], y_value);

    EXPECT_FLOAT_EQ(test_vector.xz[0], x_value);
    EXPECT_FLOAT_EQ(test_vector.xz[1], z_value);

    EXPECT_FLOAT_EQ(test_vector.yx[0], y_value);
    EXPECT_FLOAT_EQ(test_vector.yx[1], x_value);

    EXPECT_FLOAT_EQ(test_vector.yy[0], y_value);
    EXPECT_FLOAT_EQ(test_vector.yy[1], y_value);

    EXPECT_FLOAT_EQ(test_vector.yz[0], y_value);
    EXPECT_FLOAT_EQ(test_vector.yz[1], z_value);

    EXPECT_FLOAT_EQ(test_vector.zx[0], z_value);
    EXPECT_FLOAT_EQ(test_vector.zx[1], x_value);

    EXPECT_FLOAT_EQ(test_vector.zy[0], z_value);
    EXPECT_FLOAT_EQ(test_vector.zy[1], y_value);

    EXPECT_FLOAT_EQ(test_vector.zz[0], z_value);
    EXPECT_FLOAT_EQ(test_vector.zz[1], z_value);

    EXPECT_FLOAT_EQ(test_vector.xxx[0], x_value);
    EXPECT_FLOAT_EQ(test_vector.xxx[1], x_value);
    EXPECT_FLOAT_EQ(test_vector.xxx[2], x_value);

    EXPECT_FLOAT_EQ(test_vector.xxy[0], x_value);
    EXPECT_FLOAT_EQ(test_vector.xxy[1], x_value);
    EXPECT_FLOAT_EQ(test_vector.xxy[2], y_value);

    EXPECT_FLOAT_EQ(test_vector.xxz[0], x_value);
    EXPECT_FLOAT_EQ(test_vector.xxz[1], x_value);
    EXPECT_FLOAT_EQ(test_vector.xxz[2], z_value);

    EXPECT_FLOAT_EQ(test_vector.xyx[0], x_value);
    EXPECT_FLOAT_EQ(test_vector.xyx[1], y_value);
    EXPECT_FLOAT_EQ(test_vector.xyx[2], x_value);

    EXPECT_FLOAT_EQ(test_vector.xyy[0], x_value);
    EXPECT_FLOAT_EQ(test_vector.xyy[1], y_value);
    EXPECT_FLOAT_EQ(test_vector.xyy[2], y_value);

    EXPECT_FLOAT_EQ(test_vector.xyz[0], x_value);
    EXPECT_FLOAT_EQ(test_vector.xyz[1], y_value);
    EXPECT_FLOAT_EQ(test_vector.xyz[2], z_value);

    EXPECT_FLOAT_EQ(test_vector.xzx[0], x_value);
    EXPECT_FLOAT_EQ(test_vector.xzx[1], z_value);
    EXPECT_FLOAT_EQ(test_vector.xzx[2], x_value);

    EXPECT_FLOAT_EQ(test_vector.xzy[0], x_value);
    EXPECT_FLOAT_EQ(test_vector.xzy[1], z_value);
    EXPECT_FLOAT_EQ(test_vector.xzy[2], y_value);

    EXPECT_FLOAT_EQ(test_vector.xzz[0], x_value);
    EXPECT_FLOAT_EQ(test_vector.xzz[1], z_value);
    EXPECT_FLOAT_EQ(test_vector.xzz[2], z_value);

    EXPECT_FLOAT_EQ(test_vector.yxx[0], y_value);
    EXPECT_FLOAT_EQ(test_vector.yxx[1], x_value);
    EXPECT_FLOAT_EQ(test_vector.yxx[2], x_value);

    EXPECT_FLOAT_EQ(test_vector.yxy[0], y_value);
    EXPECT_FLOAT_EQ(test_vector.yxy[1], x_value);
    EXPECT_FLOAT_EQ(test_vector.yxy[2], y_value);

    EXPECT_FLOAT_EQ(test_vector.yxz[0], y_value);
    EXPECT_FLOAT_EQ(test_vector.yxz[1], x_value);
    EXPECT_FLOAT_EQ(test_vector.yxz[2], z_value);

    EXPECT_FLOAT_EQ(test_vector.yyx[0], y_value);
    EXPECT_FLOAT_EQ(test_vector.yyx[1], y_value);
    EXPECT_FLOAT_EQ(test_vector.yyx[2], x_value);

    EXPECT_FLOAT_EQ(test_vector.yyy[0], y_value);
    EXPECT_FLOAT_EQ(test_vector.yyy[1], y_value);
    EXPECT_FLOAT_EQ(test_vector.yyy[2], y_value);

    EXPECT_FLOAT_EQ(test_vector.yyz[0], y_value);
    EXPECT_FLOAT_EQ(test_vector.yyz[1], y_value);
    EXPECT_FLOAT_EQ(test_vector.yyz[2], z_value);

    EXPECT_FLOAT_EQ(test_vector.yzx[0], y_value);
    EXPECT_FLOAT_EQ(test_vector.yzx[1], z_value);
    EXPECT_FLOAT_EQ(test_vector.yzx[2], x_value);

    EXPECT_FLOAT_EQ(test_vector.yzy[0], y_value);
    EXPECT_FLOAT_EQ(test_vector.yzy[1], z_value);
    EXPECT_FLOAT_EQ(test_vector.yzy[2], y_value);

    EXPECT_FLOAT_EQ(test_vector.yzz[0], y_value);
    EXPECT_FLOAT_EQ(test_vector.yzz[1], z_value);
    EXPECT_FLOAT_EQ(test_vector.yzz[2], z_value);

    EXPECT_FLOAT_EQ(test_vector.zxx[0], z_value);
    EXPECT_FLOAT_EQ(test_vector.zxx[1], x_value);
    EXPECT_FLOAT_EQ(test_vector.zxx[2], x_value);

    EXPECT_FLOAT_EQ(test_vector.zxy[0], z_value);
    EXPECT_FLOAT_EQ(test_vector.zxy[1], x_value);
    EXPECT_FLOAT_EQ(test_vector.zxy[2], y_value);

    EXPECT_FLOAT_EQ(test_vector.zxz[0], z_value);
    EXPECT_FLOAT_EQ(test_vector.zxz[1], x_value);
    EXPECT_FLOAT_EQ(test_vector.zxz[2], z_value);

    EXPECT_FLOAT_EQ(test_vector.zyx[0], z_value);
    EXPECT_FLOAT_EQ(test_vector.zyx[1], y_value);
    EXPECT_FLOAT_EQ(test_vector.zyx[2], x_value);

    EXPECT_FLOAT_EQ(test_vector.zyy[0], z_value);
    EXPECT_FLOAT_EQ(test_vector.zyy[1], y_value);
    EXPECT_FLOAT_EQ(test_vector.zyy[2], y_value);

    EXPECT_FLOAT_EQ(test_vector.zyz[0], z_value);
    EXPECT_FLOAT_EQ(test_vector.zyz[1], y_value);
    EXPECT_FLOAT_EQ(test_vector.zyz[2], z_value);

    EXPECT_FLOAT_EQ(test_vector.zzx[0], z_value);
    EXPECT_FLOAT_EQ(test_vector.zzx[1], z_value);
    EXPECT_FLOAT_EQ(test_vector.zzx[2], x_value);

    EXPECT_FLOAT_EQ(test_vector.zzy[0], z_value);
    EXPECT_FLOAT_EQ(test_vector.zzy[1], z_value);
    EXPECT_FLOAT_EQ(test_vector.zzy[2], y_value);

    EXPECT_FLOAT_EQ(test_vector.zzz[0], z_value);
    EXPECT_FLOAT_EQ(test_vector.zzz[1], z_value);
    EXPECT_FLOAT_EQ(test_vector.zzz[2], z_value);
}

TEST(fvec4, TestAllCombinationsOfVec4)
{
    const float x_value = 0.0f;
    const float y_value = 1.0f;
    const float z_value = 2.0f;
    const float w_value = 3.0f;

    fvec4 test_vector{x_value, y_value, z_value, w_value};

    EXPECT_FLOAT_EQ(test_vector.x, x_value);
    EXPECT_FLOAT_EQ(test_vector.y, y_value);
    EXPECT_FLOAT_EQ(test_vector.z, z_value);
    EXPECT_FLOAT_EQ(test_vector.w, w_value);

    EXPECT_FLOAT_EQ(test_vector.xx[0], x_value);
    EXPECT_FLOAT_EQ(test_vector.xx[1], x_value);

    EXPECT_FLOAT_EQ(test_vector.xy[0], x_value);
    EXPECT_FLOAT_EQ(test_vector.xy[1], y_value);

    EXPECT_FLOAT_EQ(test_vector.xz[0], x_value);
    EXPECT_FLOAT_EQ(test_vector.xz[1], z_value);

    EXPECT_FLOAT_EQ(test_vector.xw[0], x_value);
    EXPECT_FLOAT_EQ(test_vector.xw[1], w_value);

    EXPECT_FLOAT_EQ(test_vector.yx[0], y_value);
    EXPECT_FLOAT_EQ(test_vector.yx[1], x_value);

    EXPECT_FLOAT_EQ(test_vector.yy[0], y_value);
    EXPECT_FLOAT_EQ(test_vector.yy[1], y_value);

    EXPECT_FLOAT_EQ(test_vector.yz[0], y_value);
    EXPECT_FLOAT_EQ(test_vector.yz[1], z_value);

    EXPECT_FLOAT_EQ(test_vector.yw[0], y_value);
    EXPECT_FLOAT_EQ(test_vector.yw[1], w_value);

    EXPECT_FLOAT_EQ(test_vector.zx[0], z_value);
    EXPECT_FLOAT_EQ(test_vector.zx[1], x_value);

    EXPECT_FLOAT_EQ(test_vector.zy[0], z_value);
    EXPECT_FLOAT_EQ(test_vector.zy[1], y_value);

    EXPECT_FLOAT_EQ(test_vector.zz[0], z_value);
    EXPECT_FLOAT_EQ(test_vector.zz[1], z_value);

    EXPECT_FLOAT_EQ(test_vector.zw[0], z_value);
    EXPECT_FLOAT_EQ(test_vector.zw[1], w_value);

    EXPECT_FLOAT_EQ(test_vector.wx[0], w_value);
    EXPECT_FLOAT_EQ(test_vector.wx[1], x_value);

    EXPECT_FLOAT_EQ(test_vector.wy[0], w_value);
    EXPECT_FLOAT_EQ(test_vector.wy[1], y_value);

    EXPECT_FLOAT_EQ(test_vector.wz[0], w_value);
    EXPECT_FLOAT_EQ(test_vector.wz[1], z_value);

    EXPECT_FLOAT_EQ(test_vector.ww[0], w_value);
    EXPECT_FLOAT_EQ(test_vector.ww[1], w_value);

    EXPECT_FLOAT_EQ(test_vector.xxx[0], x_value);
    EXPECT_FLOAT_EQ(test_vector.xxx[1], x_value);
    EXPECT_FLOAT_EQ(test_vector.xxx[2], x_value);

    EXPECT_FLOAT_EQ(test_vector.xxy[0], x_value);
    EXPECT_FLOAT_EQ(test_vector.xxy[1], x_value);
    EXPECT_FLOAT_EQ(test_vector.xxy[2], y_value);

    EXPECT_FLOAT_EQ(test_vector.xxz[0], x_value);
    EXPECT_FLOAT_EQ(test_vector.xxz[1], x_value);
    EXPECT_FLOAT_EQ(test_vector.xxz[2], z_value);

    EXPECT_FLOAT_EQ(test_vector.xxw[0], x_value);
    EXPECT_FLOAT_EQ(test_vector.xxw[1], x_value);
    EXPECT_FLOAT_EQ(test_vector.xxw[2], w_value);

    EXPECT_FLOAT_EQ(test_vector.xyx[0], x_value);
    EXPECT_FLOAT_EQ(test_vector.xyx[1], y_value);
    EXPECT_FLOAT_EQ(test_vector.xyx[2], x_value);

    EXPECT_FLOAT_EQ(test_vector.xyy[0], x_value);
    EXPECT_FLOAT_EQ(test_vector.xyy[1], y_value);
    EXPECT_FLOAT_EQ(test_vector.xyy[2], y_value);

    EXPECT_FLOAT_EQ(test_vector.xyz[0], x_value);
    EXPECT_FLOAT_EQ(test_vector.xyz[1], y_value);
    EXPECT_FLOAT_EQ(test_vector.xyz[2], z_value);

    EXPECT_FLOAT_EQ(test_vector.xyw[0], x_value);
    EXPECT_FLOAT_EQ(test_vector.xyw[1], y_value);
    EXPECT_FLOAT_EQ(test_vector.xyw[2], w_value);

    EXPECT_FLOAT_EQ(test_vector.xzx[0], x_value);
    EXPECT_FLOAT_EQ(test_vector.xzx[1], z_value);
    EXPECT_FLOAT_EQ(test_vector.xzx[2], x_value);

    EXPECT_FLOAT_EQ(test_vector.xzy[0], x_value);
    EXPECT_FLOAT_EQ(test_vector.xzy[1], z_value);
    EXPECT_FLOAT_EQ(test_vector.xzy[2], y_value);

    EXPECT_FLOAT_EQ(test_vector.xzz[0], x_value);
    EXPECT_FLOAT_EQ(test_vector.xzz[1], z_value);
    EXPECT_FLOAT_EQ(test_vector.xzz[2], z_value);

    EXPECT_FLOAT_EQ(test_vector.xzw[0], x_value);
    EXPECT_FLOAT_EQ(test_vector.xzw[1], z_value);
    EXPECT_FLOAT_EQ(test_vector.xzw[2], w_value);

    EXPECT_FLOAT_EQ(test_vector.xwx[0], x_value);
    EXPECT_FLOAT_EQ(test_vector.xwx[1], w_value);
    EXPECT_FLOAT_EQ(test_vector.xwx[2], x_value);

    EXPECT_FLOAT_EQ(test_vector.xwy[0], x_value);
    EXPECT_FLOAT_EQ(test_vector.xwy[1], w_value);
    EXPECT_FLOAT_EQ(test_vector.xwy[2], y_value);

    EXPECT_FLOAT_EQ(test_vector.xwz[0], x_value);
    EXPECT_FLOAT_EQ(test_vector.xwz[1], w_value);
    EXPECT_FLOAT_EQ(test_vector.xwz[2], z_value);

    EXPECT_FLOAT_EQ(test_vector.xww[0], x_value);
    EXPECT_FLOAT_EQ(test_vector.xww[1], w_value);
    EXPECT_FLOAT_EQ(test_vector.xww[2], w_value);

    EXPECT_FLOAT_EQ(test_vector.yxx[0], y_value);
    EXPECT_FLOAT_EQ(test_vector.yxx[1], x_value);
    EXPECT_FLOAT_EQ(test_vector.yxx[2], x_value);

    EXPECT_FLOAT_EQ(test_vector.yxy[0], y_value);
    EXPECT_FLOAT_EQ(test_vector.yxy[1], x_value);
    EXPECT_FLOAT_EQ(test_vector.yxy[2], y_value);

    EXPECT_FLOAT_EQ(test_vector.yxz[0], y_value);
    EXPECT_FLOAT_EQ(test_vector.yxz[1], x_value);
    EXPECT_FLOAT_EQ(test_vector.yxz[2], z_value);

    EXPECT_FLOAT_EQ(test_vector.yxw[0], y_value);
    EXPECT_FLOAT_EQ(test_vector.yxw[1], x_value);
    EXPECT_FLOAT_EQ(test_vector.yxw[2], w_value);

    EXPECT_FLOAT_EQ(test_vector.yyx[0], y_value);
    EXPECT_FLOAT_EQ(test_vector.yyx[1], y_value);
    EXPECT_FLOAT_EQ(test_vector.yyx[2], x_value);

    EXPECT_FLOAT_EQ(test_vector.yyy[0], y_value);
    EXPECT_FLOAT_EQ(test_vector.yyy[1], y_value);
    EXPECT_FLOAT_EQ(test_vector.yyy[2], y_value);

    EXPECT_FLOAT_EQ(test_vector.yyz[0], y_value);
    EXPECT_FLOAT_EQ(test_vector.yyz[1], y_value);
    EXPECT_FLOAT_EQ(test_vector.yyz[2], z_value);

    EXPECT_FLOAT_EQ(test_vector.yyw[0], y_value);
    EXPECT_FLOAT_EQ(test_vector.yyw[1], y_value);
    EXPECT_FLOAT_EQ(test_vector.yyw[2], w_value);

    EXPECT_FLOAT_EQ(test_vector.yzx[0], y_value);
    EXPECT_FLOAT_EQ(test_vector.yzx[1], z_value);
    EXPECT_FLOAT_EQ(test_vector.yzx[2], x_value);

    EXPECT_FLOAT_EQ(test_vector.yzy[0], y_value);
    EXPECT_FLOAT_EQ(test_vector.yzy[1], z_value);
    EXPECT_FLOAT_EQ(test_vector.yzy[2], y_value);

    EXPECT_FLOAT_EQ(test_vector.yzz[0], y_value);
    EXPECT_FLOAT_EQ(test_vector.yzz[1], z_value);
    EXPECT_FLOAT_EQ(test_vector.yzz[2], z_value);

    EXPECT_FLOAT_EQ(test_vector.yzw[0], y_value);
    EXPECT_FLOAT_EQ(test_vector.yzw[1], z_value);
    EXPECT_FLOAT_EQ(test_vector.yzw[2], w_value);

    EXPECT_FLOAT_EQ(test_vector.ywx[0], y_value);
    EXPECT_FLOAT_EQ(test_vector.ywx[1], w_value);
    EXPECT_FLOAT_EQ(test_vector.ywx[2], x_value);

    EXPECT_FLOAT_EQ(test_vector.ywy[0], y_value);
    EXPECT_FLOAT_EQ(test_vector.ywy[1], w_value);
    EXPECT_FLOAT_EQ(test_vector.ywy[2], y_value);

    EXPECT_FLOAT_EQ(test_vector.ywz[0], y_value);
    EXPECT_FLOAT_EQ(test_vector.ywz[1], w_value);
    EXPECT_FLOAT_EQ(test_vector.ywz[2], z_value);

    EXPECT_FLOAT_EQ(test_vector.yww[0], y_value);
    EXPECT_FLOAT_EQ(test_vector.yww[1], w_value);
    EXPECT_FLOAT_EQ(test_vector.yww[2], w_value);

    EXPECT_FLOAT_EQ(test_vector.zxx[0], z_value);
    EXPECT_FLOAT_EQ(test_vector.zxx[1], x_value);
    EXPECT_FLOAT_EQ(test_vector.zxx[2], x_value);

    EXPECT_FLOAT_EQ(test_vector.zxy[0], z_value);
    EXPECT_FLOAT_EQ(test_vector.zxy[1], x_value);
    EXPECT_FLOAT_EQ(test_vector.zxy[2], y_value);

    EXPECT_FLOAT_EQ(test_vector.zxz[0], z_value);
    EXPECT_FLOAT_EQ(test_vector.zxz[1], x_value);
    EXPECT_FLOAT_EQ(test_vector.zxz[2], z_value);

    EXPECT_FLOAT_EQ(test_vector.zxw[0], z_value);
    EXPECT_FLOAT_EQ(test_vector.zxw[1], x_value);
    EXPECT_FLOAT_EQ(test_vector.zxw[2], w_value);

    EXPECT_FLOAT_EQ(test_vector.zyx[0], z_value);
    EXPECT_FLOAT_EQ(test_vector.zyx[1], y_value);
    EXPECT_FLOAT_EQ(test_vector.zyx[2], x_value);

    EXPECT_FLOAT_EQ(test_vector.zyy[0], z_value);
    EXPECT_FLOAT_EQ(test_vector.zyy[1], y_value);
    EXPECT_FLOAT_EQ(test_vector.zyy[2], y_value);

    EXPECT_FLOAT_EQ(test_vector.zyz[0], z_value);
    EXPECT_FLOAT_EQ(test_vector.zyz[1], y_value);
    EXPECT_FLOAT_EQ(test_vector.zyz[2], z_value);

    EXPECT_FLOAT_EQ(test_vector.zyw[0], z_value);
    EXPECT_FLOAT_EQ(test_vector.zyw[1], y_value);
    EXPECT_FLOAT_EQ(test_vector.zyw[2], w_value);

    EXPECT_FLOAT_EQ(test_vector.zzx[0], z_value);
    EXPECT_FLOAT_EQ(test_vector.zzx[1], z_value);
    EXPECT_FLOAT_EQ(test_vector.zzx[2], x_value);

    EXPECT_FLOAT_EQ(test_vector.zzy[0], z_value);
    EXPECT_FLOAT_EQ(test_vector.zzy[1], z_value);
    EXPECT_FLOAT_EQ(test_vector.zzy[2], y_value);

    EXPECT_FLOAT_EQ(test_vector.zzz[0], z_value);
    EXPECT_FLOAT_EQ(test_vector.zzz[1], z_value);
    EXPECT_FLOAT_EQ(test_vector.zzz[2], z_value);

    EXPECT_FLOAT_EQ(test_vector.zzw[0], z_value);
    EXPECT_FLOAT_EQ(test_vector.zzw[1], z_value);
    EXPECT_FLOAT_EQ(test_vector.zzw[2], w_value);

    EXPECT_FLOAT_EQ(test_vector.zwx[0], z_value);
    EXPECT_FLOAT_EQ(test_vector.zwx[1], w_value);
    EXPECT_FLOAT_EQ(test_vector.zwx[2], x_value);

    EXPECT_FLOAT_EQ(test_vector.zwy[0], z_value);
    EXPECT_FLOAT_EQ(test_vector.zwy[1], w_value);
    EXPECT_FLOAT_EQ(test_vector.zwy[2], y_value);

    EXPECT_FLOAT_EQ(test_vector.zwz[0], z_value);
    EXPECT_FLOAT_EQ(test_vector.zwz[1], w_value);
    EXPECT_FLOAT_EQ(test_vector.zwz[2], z_value);

    EXPECT_FLOAT_EQ(test_vector.zww[0], z_value);
    EXPECT_FLOAT_EQ(test_vector.zww[1], w_value);
    EXPECT_FLOAT_EQ(test_vector.zww[2], w_value);

    compare_all_vec4_x_compinations(test_vector, x_value, y_value, z_value, w_value);
}
