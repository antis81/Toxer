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

#include "ToxerPrivate.h"

/**
@class ToxerPrivate

@brief Tox API functions.


@fn ToxerPrivate::profilesDir
@brief Absolute path to the location of Tox profiles and common data.


@fn ToxerPrivate::pk
@brief Returns a friend's public key.
@param[in] tox          the valid Tox instance
@param[in] friendIndex  the friend index
@return the public key as raw data or an empty byte array

The profile's public key can be returned by passing -1 as the friendIndex.
*/

/**
@brief ToxerPrivate::toxErrStr
@param err
@return
*/
const char* ToxerPrivate::toxErrStr(int err)
{
    switch (err) {
    case TOX_ERR_NEW_OK: return "no error";
    case TOX_ERR_NEW_NULL: return "One of the arguments was NULL.";
    case TOX_ERR_NEW_MALLOC: return "out of memory";

    case TOX_ERR_NEW_LOAD_BAD_FORMAT: return "invalid savefile format";
    case TOX_ERR_NEW_LOAD_ENCRYPTED: return  "decryption failed";
    case TOX_ERR_NEW_PORT_ALLOC: return "out of resources";
    case TOX_ERR_NEW_PROXY_BAD_HOST: return "invalid proxy hostname";
    case TOX_ERR_NEW_PROXY_BAD_PORT: return "invalid proxy port";
    case TOX_ERR_NEW_PROXY_BAD_TYPE: return "proxy type not supported";
    case TOX_ERR_NEW_PROXY_NOT_FOUND: return "proxy host not found";

    // TODO: complete the error type list
    }

    return "unknown error";
}

bool ToxerPrivate::isEncrypted(const char* data)
{
    const uint8_t* c_data = reinterpret_cast<const uint8_t*>(data);
    return tox_is_data_encrypted(c_data);
}

ToxerPrivate::PassKeyPtr ToxerPrivate::createKey(const char* data, int len,
                                                 const char* salt)
{
    const uint8_t* c_salt = reinterpret_cast<const uint8_t*>(salt);
    const uint8_t* c_data = reinterpret_cast<const uint8_t*>(data);
    size_t c_len = static_cast<size_t>(len);
    Tox_Pass_Key* k =
            salt ? tox_pass_key_derive_with_salt(c_data, c_len, c_salt, nullptr)
                 : tox_pass_key_derive(c_data, c_len, nullptr);
    return PassKeyPtr(k, tox_pass_key_free);
}

/**
@brief Encrypts a data array.
*/
QByteArray ToxerPrivate::encrypt(const char* rawData, int len,
                                 const ToxerPrivate::PassKeyPtr key)
{
    const uint8_t* c_data = reinterpret_cast<const uint8_t*>(rawData);
    size_t c_len = static_cast<size_t>(len);

    QByteArray encrypted(len + TOX_PASS_ENCRYPTION_EXTRA_LENGTH, 0);
    uint8_t* c_encrypted = reinterpret_cast<uint8_t*>(encrypted.data());

    if (!tox_pass_key_encrypt(key.get(), c_data, c_len, c_encrypted, nullptr))
    {
        qWarning("Encryption failed!");
        return {};
    }

    return encrypted;
}

/**
@brief Decrypts a data array.
*/
QByteArray ToxerPrivate::decrypt(const char* encrypted, int len,
                                 const PassKeyPtr key)
{
    const uint8_t* c_encrypted = reinterpret_cast<const uint8_t*>(encrypted);
    size_t c_len = static_cast<size_t>(len);

    const int decryptedLen = qMax(0, len - TOX_PASS_ENCRYPTION_EXTRA_LENGTH);
    QByteArray decrypted(decryptedLen, 0);
    uint8_t* c_decrypted = reinterpret_cast<uint8_t*>(decrypted.data());

    bool ok = tox_pass_key_decrypt(key.get(), c_encrypted, c_len, c_decrypted,
                                   nullptr);
    if (ok)
    {
        return decrypted;
    } else {
        qWarning("Decryption failed!");
        return {};
    }
}

/**
@brief Decrypts a data array using a salted encryption key.

The encryption key is generated from the UTF-8 encoded password.
*/
QByteArray ToxerPrivate::decrypt(const char* encrypted, int len,
                                 const QString& password)
{
    char salt[TOX_PASS_SALT_LENGTH];
    const uint8_t* c_encrypted = reinterpret_cast<const uint8_t*>(encrypted);
    uint8_t* c_salt = reinterpret_cast<uint8_t*>(salt);
    tox_get_salt(c_encrypted, c_salt, nullptr);

    const QByteArray pw = password.toUtf8();
    const ToxerPrivate::PassKeyPtr generated_key = createKey(pw.constData(),
                                                             pw.length(),
                                                             salt);

    return decrypt(encrypted, len, generated_key);
}

/**
@brief conversion from Tox to Qt
*/
ToxTypes::Proxy ToxerPrivate::fromTox(TOX_PROXY_TYPE enumeration)
{
    switch (enumeration) {
    case TOX_PROXY_TYPE_NONE: return ToxTypes::Proxy::None;
    case TOX_PROXY_TYPE_HTTP: return ToxTypes::Proxy::HTTP;
    case TOX_PROXY_TYPE_SOCKS5: return ToxTypes::Proxy::SOCKS5;
    }

    assert(false);
    return ToxTypes::Proxy::None;
}

/**
@brief conversion from Qt to Tox
*/
TOX_PROXY_TYPE ToxerPrivate::toTox(ToxTypes::Proxy enumeration)
{
    switch (enumeration) {
    case ToxTypes::Proxy::None: return TOX_PROXY_TYPE_NONE;
    case ToxTypes::Proxy::HTTP: return TOX_PROXY_TYPE_HTTP;
    case ToxTypes::Proxy::SOCKS5: return TOX_PROXY_TYPE_SOCKS5;
    }

    assert(false);
    return TOX_PROXY_TYPE_NONE;
}

/**
@brief conversion from Qt to Tox
*/
ToxTypes::UserStatus ToxerPrivate::fromTox(TOX_USER_STATUS enumeration)
{
    switch (enumeration) {
    case TOX_USER_STATUS_NONE: return ToxTypes::UserStatus::Ready;
    case TOX_USER_STATUS_AWAY: return ToxTypes::UserStatus::Away;
    case TOX_USER_STATUS_BUSY: return ToxTypes::UserStatus::Busy;
    }

    assert(false);
    return ToxTypes::UserStatus::Unknown;
}

/**
@brief conversion from Qt to Tox
*/
TOX_USER_STATUS ToxerPrivate::toTox(ToxTypes::UserStatus enumeration)
{
    switch (enumeration) {
    case ToxTypes::UserStatus::Ready: return TOX_USER_STATUS_NONE;
    case ToxTypes::UserStatus::Busy: return TOX_USER_STATUS_BUSY;
    case ToxTypes::UserStatus::Away:
    case ToxTypes::UserStatus::Unknown: return TOX_USER_STATUS_AWAY;
    }

    assert(false);
    return TOX_USER_STATUS_AWAY;
}
