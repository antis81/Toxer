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

#include "ToxProfile.h"

#include "ToxBootstrap.h"
#include "Settings.h"
#include "IToxNotify.h"

#include <QFile>

ToxProfilePrivate* ToxProfilePrivate::activeProfile = nullptr;

/**
@class ToxProfilePrivate
@brief Tox profile Qt implementation.
*/

/**
@brief Creates a Tox profile.
@param name         the profile name
@param password     the profile password
*/
void ToxProfilePrivate::create(const QString& name, const QString& password)
{
    QFile f(ToxerPrivate::profilesDir() % QString::fromUtf8("/%1.tox")
            .arg(name));

    if (f.exists()) {
        qWarning("Exiting Tox profile found at \"%s\"",
                 qUtf8Printable(f.fileName()));
        return;
    }

    if (f.open(QFile::WriteOnly)) {
        Tox* t = createTox({});
        if (t) {
            size_t c_size = tox_get_savedata_size(t);
            uint8_t* c_data = new uint8_t[c_size];
            tox_get_savedata(t, c_data);

            TOX_ERR_ENCRYPTION err = TOX_ERR_ENCRYPTION_OK;
            size_t c_encrypted_size = c_size + TOX_PASS_ENCRYPTION_EXTRA_LENGTH;
            uint8_t* c_encrypted = new uint8_t[c_encrypted_size];
            QByteArray pw = password.toUtf8();
            tox_pass_encrypt(c_data, c_size,
                             reinterpret_cast<const uint8_t*>(pw.constData()),
                             static_cast<size_t>(pw.length()), c_encrypted,
                             &err);

            if (err == TOX_ERR_ENCRYPTION_OK) {
                f.write(reinterpret_cast<char*>(c_encrypted),
                        static_cast<qint64>(c_encrypted_size));
            } else {
                qWarning("Encryption of Tox profile failed.");
            }

            delete[] c_data;
            delete[] c_encrypted;
            tox_kill(t);
        }
    } else {
        qWarning("Failed to create Tox profile %s.\nError message: %s",
                 qUtf8Printable(name),
                 qUtf8Printable(f.errorString()));
    }
}

/**
@brief Loads a Tox profile.
@param[in] name         the profile name
@param[in] password     the profile password
@return the profile data
*/
QByteArray ToxProfilePrivate::load(const QString& name, const QString& password)
{
    QFile f(ToxerPrivate::profilesDir() % QString::fromUtf8("/%1.tox")
            .arg(name));

    if (!f.exists()) {
        qWarning("No Tox savefile found at \"%s\"",
                 qUtf8Printable(f.fileName()));
        return {};
    }

    if (f.open(QFile::ReadOnly)) {
        const QByteArray data = f.readAll();
        const QByteArray decrypted =
                ToxerPrivate::isEncrypted(data.constData())
                ? ToxerPrivate::decrypt(data.constData(), data.length(),
                                        password)
                : data;

        return decrypted;
    } else {
        qWarning("Failed to open Tox profile %s.\nError message: %s",
                 qUtf8Printable(name),
                 qUtf8Printable(f.errorString()));

        return {};
    }
}

/**
@brief Activates a Tox profile.
@note A previously active profile will be closed.
*/
void ToxProfilePrivate::activate(const QString& profileName,
                          const QString& password)
{
    if (activeProfile && activeProfile->name() == profileName) {
        return;
    }

    delete activeProfile;

    QByteArray profileData = load(profileName, password);
    if (!profileData.isEmpty()) {
        activeProfile = new ToxProfilePrivate(profileName, profileData);
        activeProfile->start();
    } else {
        qWarning("Tox profile not activated: Wrong password.");
    }

    return;
}

/**
@brief Creates a Tox instance for the profile.
@param profileData  the saved profile state
@return the created Tox instance
*/
Tox* ToxProfilePrivate::createTox(const QByteArray& profileData)
{
    ToxSettings toxSettings;

    Tox_Options toxOpts;
    tox_options_default(&toxOpts);
    toxOpts.ipv6_enabled = toxSettings.ipv6_enabled();
    toxOpts.udp_enabled = toxSettings.udp_enabled();

    if (!profileData.isEmpty()) {
        toxOpts.savedata_type = TOX_SAVEDATA_TYPE_TOX_SAVE;
        toxOpts.savedata_data =
                reinterpret_cast<const uint8_t*>(profileData.constData());
        toxOpts.savedata_length = static_cast<size_t>(profileData.size());
    }

    ToxTypes::Proxy proxyType = toxSettings.proxy_type();
    if (proxyType != ToxTypes::Proxy::None)
    {
        QByteArray proxyAddr = toxSettings.proxy_addr().toUtf8();
        quint16 proxyPort = toxSettings.proxy_port();
        if (proxyAddr.length() > 255)
        {
            qWarning("The proxy address %s is too long.",
                     qUtf8Printable(toxSettings.proxy_addr()));
        }
        else if (!proxyAddr.isEmpty() && toxSettings.proxy_port() > 0)
        {
            toxOpts.proxy_type = ToxerPrivate::toTox(proxyType);
            toxOpts.proxy_host = proxyAddr.constData();
            toxOpts.proxy_port = proxyPort;
        }
    }

    TOX_ERR_NEW err;
    Tox* tox = tox_new(&toxOpts, &err);
    if (err != TOX_ERR_NEW_OK) {
        qWarning("Creation of Tox instance failed: %s",
                 ToxerPrivate::toxErrStr(err));
    }

    return tox;
}

ToxProfilePrivate::ToxEventLoop::ToxEventLoop(Tox* _tox)
    : QThread()
    , tox(_tox)
{
    assert(tox);
    setObjectName(QStringLiteral("ToxEventLoop"));
    connect(this, &QThread::finished, this, &QThread::deleteLater);
}

void ToxProfilePrivate::ToxEventLoop::bootstrap()
{
    assert(tox);
    assert(!isRunning());

    quint32 j = static_cast<quint32>(qrand());

    for (int i = 0; i < 4; i++, j++) {
        const BootstrapNode *d =
                &bootstrap_nodes[j % COUNTOF(bootstrap_nodes)];
        TOX_ERR_BOOTSTRAP err = TOX_ERR_BOOTSTRAP_OK;
        tox_bootstrap(tox, d->address, d->port, d->key, &err);
        if (err) {
            qWarning("Failed to bootstrap address %s. Code: %d",
                     d->address, err);
        }

        tox_add_tcp_relay(tox, d->address, d->port, d->key, &err);
        if (err) {
            qWarning("Failed to add TCP relay for %s. Code: %d",
                     d->address, err);
        }
    }
}

void ToxProfilePrivate::ToxEventLoop::run()
{
    QMutexLocker locker(&mutex);
    const size_t interval = tox_iteration_interval(tox);
    locker.unlock();

    while (isRunning()) {
        tox_iterate(tox, nullptr);
        msleep(interval);
    }

    locker.relock();
    tox_kill(tox);
}

ToxProfilePrivate::ToxProfilePrivate(const QString& name,
                                     const QByteArray& profileData)
    : mName(name)
    , mTEL(new ToxEventLoop(createTox(profileData)))
{
    tox_callback_self_connection_status(mTEL->tox,
                                        [](Tox*, TOX_CONNECTION status, void*)
    {
        if (activeProfile) {
            for (auto n : activeProfile->profileNotifiers) {
                n->on_is_online_changed(status != TOX_CONNECTION_NONE);
            }
        }
    });

    tox_callback_friend_connection_status(mTEL->tox,
                                          [](Tox*, uint32_t c_index,
                                          TOX_CONNECTION status, void*)
    {
        if (activeProfile){
            int index = static_cast<int>(c_index);
            for (auto n : activeProfile->friendNotifiers) {
                n->on_is_online_changed(index, status != TOX_CONNECTION_NONE);
            }
        }
    });

    tox_callback_friend_name(mTEL->tox,
                             [](Tox*, uint32_t c_index,
                             const uint8_t* c_name, size_t c_len, void*)
    {
        if (activeProfile) {
            int index = static_cast<int>(c_index);
            const char* name = reinterpret_cast<const char*>(c_name);
            int len = static_cast<int>(c_len);
            QString nameStr = QString::fromUtf8(name, len);
            for (auto n : activeProfile->friendNotifiers) {
                n->on_name_changed(index, nameStr);
            }
        }
    });

    tox_callback_friend_status_message(mTEL->tox,
                                       [](Tox*, uint32_t c_index,
                                       const uint8_t* c_message, size_t c_len,
                                       void*)
    {
        if (activeProfile) {
            int index = static_cast<int>(c_index);
            const char* message = reinterpret_cast<const char*>(c_message);
            int len = static_cast<int>(c_len);
            QString messageStr = QString::fromUtf8(message, len);
            for (auto n : activeProfile->friendNotifiers) {
                n->on_status_message_changed(index, messageStr);
            }
        }
    });

    tox_callback_friend_status(mTEL->tox,
                               [](Tox*, uint32_t c_index,
                               TOX_USER_STATUS status, void*)
    {
        if (activeProfile) {
            int index = static_cast<int>(c_index);
            for (auto n : activeProfile->friendNotifiers) {
                n->on_status_changed(index, static_cast<quint8>(status));
            }
        }
    });

    tox_callback_friend_message(mTEL->tox, [](Tox*, uint32_t c_index,
                                TOX_MESSAGE_TYPE type, const uint8_t *c_message,
                                size_t c_len, void *) {
        // TODO: handle message type
        Q_UNUSED(type);

        // TODO: manage message history
        if (activeProfile) {
            int index = static_cast<int>(c_index);
            const char* message = reinterpret_cast<const char*>(c_message);
            int len = static_cast<int>(c_len);
            QString messageStr = QString::fromUtf8(message, len);
            for (auto n : activeProfile->friendNotifiers) {
                n->on_message(index, messageStr);
            }
        }
    });
}

ToxProfilePrivate::~ToxProfilePrivate()
{
    mTEL->quit();
    mTEL->wait();
}

void ToxProfilePrivate::start()
{
    if (!mTEL->isRunning()) {
        mTEL->bootstrap();
        mTEL->start();
    }
}

QVariant ToxProfilePrivate::toxQuery(ToxProfilePrivate::ToxFunc query_func) const {
    QMutexLocker locker(&mTEL->mutex);
    return query_func(mTEL->tox);
}

void ToxProfilePrivate::toxSet(ToxProfilePrivate::ToxSetFunc set_func) {
    QMutexLocker locker(&mTEL->mutex);
    set_func(mTEL->tox);
}

void ToxProfilePrivate::addNotificationObserver(IToxFriendNotifier* notify)
{
    friendNotifiers << notify;
}

void ToxProfilePrivate::removeNotificationObserver(IToxFriendNotifier* notify)
{
    friendNotifiers.removeAll(notify);
}

void ToxProfilePrivate::addNotificationObserver(IToxProfileNotifier* notify)
{
    profileNotifiers << notify;
}

void ToxProfilePrivate::removeNotificationObserver(IToxProfileNotifier* notify)
{
    profileNotifiers.removeAll(notify);
}

void ToxProfilePrivate::on_status_changed(int status)
{
    if (activeProfile) {
        for (auto n : activeProfile->profileNotifiers) {
            n->on_status_changed(status != TOX_CONNECTION_NONE);
        }
    }
}

void ToxProfilePrivate::on_status_message_changed(const QString& message)
{
    if (activeProfile) {
        for (auto n : activeProfile->profileNotifiers) {
            n->on_status_message_changed(message);
        }
    }
}

void ToxProfilePrivate::on_user_name_changed(const QString& userName)
{
    if (activeProfile) {
        for (auto n : activeProfile->profileNotifiers) {
            n->on_user_name_changed(userName);
        }
    }
}
