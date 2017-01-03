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

#ifndef TOXER_PRIVATE_H
#define TOXER_PRIVATE_H

#include <ToxTypes.h>

#include <tox/tox.h>
#include <tox/toxencryptsave.h>

#include <cassert>
#include <memory>

#include <QStandardPaths>
#include <QStringBuilder>

// TODO: implement error handling for toxcore api calls

#define COUNTOF(x) (sizeof(x) / sizeof(*(x)))

class ToxerPrivate final
{
public:
    using PassKeyPtr = std::shared_ptr<Tox_Pass_Key>;

private:
    ToxerPrivate() = delete;
    ~ToxerPrivate() = delete;
    ToxerPrivate(const ToxerPrivate& other) = delete;
    ToxerPrivate(ToxerPrivate&& other) = delete;

    ToxerPrivate& operator=(const ToxerPrivate& other) = delete;
    ToxerPrivate& operator=(ToxerPrivate&& other) = delete;

public:
    inline static QString profilesDir()
    {
        return QStandardPaths::writableLocation(
                    QStandardPaths::GenericConfigLocation) %
                QStringLiteral("/tox");
    }

public:
    static const char* toxErrStr(int err);
    static bool isEncrypted(const char* data);
    static PassKeyPtr createKey(const char* data, int len, const char* salt);
    static QByteArray encrypt(const char* rawData, int len,
                              const PassKeyPtr key);
    static QByteArray decrypt(const char* encrypted, int len,
                              const PassKeyPtr key);
    static QByteArray decrypt(const char* encrypted, int len,
                              const QString& password);

public:
    static ToxTypes::Proxy fromTox(TOX_PROXY_TYPE enumeration);
    static TOX_PROXY_TYPE toTox(ToxTypes::Proxy enumeration);

    static ToxTypes::ConnectionType fromTox(TOX_CONNECTION enumeration);
    static TOX_CONNECTION toTox(ToxTypes::ConnectionType enumeration);

    static ToxTypes::UserStatus fromTox(TOX_USER_STATUS enumeration);
    static TOX_USER_STATUS toTox(ToxTypes::UserStatus enumeration);
};

#endif
