/*
 * This file is part of the Toxer application, a Tox messenger client.
 *
 * Copyright (c) 2017 Nils Fenner <nils@macgitver.org>
 *
 * This software is licensed under the terms of the MIT license:
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#ifndef TOXER_TOXTYPES_H
#define TOXER_TOXTYPES_H

#include <QMetaEnum>
#include <QQmlEngine>

/**
@brief Qt abstraction of Tox enumeration types.
*/
struct ToxTypes final
{
    Q_GADGET
public:
    enum class Proxy : quint8 { None, HTTP, SOCKS5 };
    Q_ENUM(Proxy)

    enum class ConnectionType : quint8 { None, TCP, UDP };
    Q_ENUM(ConnectionType)

    enum class UserStatus : quint8 { Unknown, Ready, Away, Busy };
    Q_ENUM(UserStatus)

public:
    inline static void registerQmlTypes() {
        qRegisterMetaType<UserStatus>();
        qRegisterMetaType<ConnectionType>();
        qRegisterMetaType<Proxy>();

        qmlRegisterUncreatableType<ToxTypes>(
                    "com.tox.qmltypes", 1, 0, "ToxTypes",
                    QStringLiteral("enumeration types cannot be created"));
    }

    template<typename T>
    inline static QVariant toQVariant(const T& enumeration) {
        return QVariant::fromValue<T>(enumeration);
    }
};

#endif
