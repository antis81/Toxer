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

#include "Toxer.h"

#include <Private/ToxProfile.h>
#include <Settings.h>

#include <QDir>
#include <QFileInfo>
#include <QGuiApplication>

QUrl Toxer::profileSelector()
{
    return QUrl(QStringLiteral("qrc:/qml/Profiles.qml"));
}

QUrl Toxer::mainView()
{
    UiSettings s;
    UiSettings::AppLayout l = s.app_layout();
    switch (l) {
    case UiSettings::AppLayout::Split:
        return QUrl(QStringLiteral("qrc:/qml/MainViewSplit.qml"));
    case UiSettings::AppLayout::Slim: break;
    }

    // default
    return QUrl(QStringLiteral("qrc:/qml/MainViewSlim.qml"));
}

Toxer::Toxer()
    : QObject()
{
}

QString Toxer::toxVersionString()
{
    return QString::fromLatin1("%1.%2.%3")
            .arg(TOX_VERSION_MAJOR)
            .arg(TOX_VERSION_MINOR)
            .arg(TOX_VERSION_PATCH);
}

QStringList Toxer::availableProfiles() const
{
    QDir dir(ToxerPrivate::profilesDir());
    dir.setFilter(QDir::Files | QDir::NoDotAndDotDot);
    dir.setNameFilters(QStringList() << QStringLiteral("*.tox"));
    QFileInfoList list = dir.entryInfoList();

    QStringList out;
    for (QFileInfo info : list)
    {
        out << info.completeBaseName();
    }

    return out;
}

/**
@brief Activates a Tox profile.
@param[in] profileName  the profile name
*/
void Toxer::activateProfile(const QString& profileName,
                            const QString& password)
{
    const ToxProfilePrivate* old = ToxProfilePrivate::current();
    ToxProfilePrivate::activate(profileName, password);
    if (old != ToxProfilePrivate::current()) {
        emit reloadUi();
    }
}

/**
@brief Creates a Tox profile.
@param profileName  the profile name
@param password     the profile password
*/
void Toxer::createProfile(const QString& profileName, const QString& password)
{
    ToxProfilePrivate::create(profileName, password);
}

/**
@brief Returns the URL to the local avatars directory.
@returns the local url to avatars directory
*/
QUrl Toxer::avatarsUrl() const
{
    return QUrl::fromLocalFile(ToxerPrivate::profilesDir() +
                               QStringLiteral("/avatars"));
}

/**
@brief Checks, if a URL points to an existing local file or directory.
@param url     the URL to check
@returns true, if file or directory exists; false otherwise
*/
bool Toxer::exists(const QUrl& url) const
{
    return QFileInfo::exists(url.toLocalFile());
}

/**
@brief ToxProfileQuery constructor
@param[in] parent
*/
ToxProfileQuery::ToxProfileQuery(QObject* parent)
    : QObject(parent)
{
}

/**
@brief profile user name changed notifier
@param userName     the user name
*/
void ToxProfileQuery::on_user_name_changed(const QString& userName)
{
    emit userNameChanged(userName);
}

/**
@brief profile is online changed notifier
@param online   the online status
*/
void ToxProfileQuery::on_is_online_changed(bool online)
{
    emit isOnlineChanged(online);
}

/**
@brief profile status message changed notifier
@param message  the status message
*/
void ToxProfileQuery::on_status_message_changed(const QString& message)
{
    emit statusMessageChanged(message);
}

/**
@brief profile status changed notifier
@param status   the user status
*/
void ToxProfileQuery::on_status_changed(int status)
{
    emit statusChanged(status);
}

/**
@brief Returns the profile name.
@return the valid profile name or an empty string

The profile name represents the base name of a '.tox' file.
*/
QString ToxProfileQuery::name() const
{
    ToxProfilePrivate* p = ToxProfilePrivate::current();
    return p ? p->name() : QString();
}

/**
@brief Returns the local profile's user name.
@return the user name
*/
QString ToxProfileQuery::userName() const
{
    const ToxProfilePrivate* p = ToxProfilePrivate::current();
    if (p) {
        return p->toxQuery([](const Tox* tox) -> QVariant {
            size_t len = tox_self_get_name_size(tox);
            char* out = new char[len + 1];
            uint8_t* c_out = reinterpret_cast<uint8_t*>(out);

            tox_self_get_name(tox, c_out);
            out[len] = '\0';

            QString outStr = QString::fromUtf8(out);
            delete[] out;

            return outStr;
        }).toString();
    } else {
        return {};
    }
}

/**
@brief Sets the local profile'S user name.
@param newValue     the user name
*/
void ToxProfileQuery::setUserName(const QString& newValue)
{
    ToxProfilePrivate* p = ToxProfilePrivate::current();
    QString oldValue = userName();
    if (p && newValue != oldValue) {
        p->toxSet([&p, &newValue](Tox* tox) {
            QByteArray str = newValue.toUtf8();
            const uint8_t* c_str =
                    reinterpret_cast<const uint8_t*>(str.constData());
            size_t c_len = static_cast<size_t>(newValue.length());
            TOX_ERR_SET_INFO err = TOX_ERR_SET_INFO_OK;
            tox_self_set_name(tox, c_str, c_len, &err);
            if (err) {
                qWarning("Could not set user name: %s",
                         ToxerPrivate::toxErrStr(err));
            } else {
                p->on_user_name_changed(newValue);
            }
        });
    }
}

/**
@brief Returns the local profile's user status message.
@return the status message
*/
QString ToxProfileQuery::statusMessage() const
{
    const ToxProfilePrivate* p = ToxProfilePrivate::current();
    if (p) {
        return p->toxQuery([](const Tox* tox) -> QVariant {
            size_t len = tox_self_get_status_message_size(tox);
            char* out = new char[len + 1];
            uint8_t* c_out = reinterpret_cast<uint8_t*>(out);

            tox_self_get_status_message(tox, c_out);
            out[len] = '\0';

            QString outStr = QString::fromUtf8(out);
            delete[] out;

            return outStr;
        }).toString();
    } else {
        return {};
    }
}

/**
@brief Sets the local profile's user status message.
@param newValue     the status message
*/
void ToxProfileQuery::setStatusMessage(const QString& newValue)
{
    ToxProfilePrivate* p = ToxProfilePrivate::current();
    QString oldValue = userName();
    if (p && newValue != oldValue) {
        p->toxSet([&p, &newValue](Tox* tox) {
            QByteArray str = newValue.toUtf8();
            const uint8_t* c_str =
                    reinterpret_cast<const uint8_t*>(str.constData());
            size_t c_len = static_cast<size_t>(newValue.length());
            TOX_ERR_SET_INFO err = TOX_ERR_SET_INFO_OK;
            tox_self_set_status_message(tox, c_str, c_len, &err);
            if (err) {
                qWarning("Could not set user name: %s",
                         ToxerPrivate::toxErrStr(err));
            } else {
                p->on_status_message_changed(newValue);
            }
        });
    }
}

/**
@brief Returns the local profile's user public key string.
@return the public key as a null-terminated hex-string
*/
QString ToxProfileQuery::publicKeyStr() const
{
    const ToxProfilePrivate* p = ToxProfilePrivate::current();
    if (p) {
        return p->toxQuery([](const Tox* tox) -> QVariant {
            return QString::fromLatin1(ToxerPrivate::pk(tox, -1).toHex());
        }).toString();
    } else {
        return {};
    }
}

/**
@brief Returns the local profile's user nospam value.
@return the nospam value
*/
quint32 ToxProfileQuery::nospam() const
{
    const ToxProfilePrivate* p = ToxProfilePrivate::current();
    if (p) {
        return p->toxQuery([](const Tox* tox) -> QVariant {
            uint32_t out = tox_self_get_nospam(tox);
            return out;
        }).toUInt();
    } else {
        return {};
    }
}

/**
@brief Returns the local profile's online status.
@return true if online; false otherwise
*/
bool ToxProfileQuery::isOnline() const
{
    const ToxProfilePrivate* p = ToxProfilePrivate::current();
    if (p) {
        return p->toxQuery([](const Tox* tox) -> QVariant {
            TOX_CONNECTION out = tox_self_get_connection_status(tox);
            return out != TOX_CONNECTION_NONE;
        }).toBool();
    } else {
        return false;
    }
}

/**
@brief Returns the local profile's user status.
@return the user status
*/
ToxTypes::UserStatus ToxProfileQuery::status() const
{
    const ToxProfilePrivate* p = ToxProfilePrivate::current();
    if (p) {
        QVariant t = p->toxQuery([](const Tox* tox) -> QVariant {
            TOX_USER_STATUS out = tox_self_get_status(tox);
            return ToxTypes::toQVariant(ToxerPrivate::fromTox(out));
        });

        return t.value<ToxTypes::UserStatus>();
    } else {
        return ToxTypes::UserStatus::Away;
    }
}

/**
@brief Returns the local profile's user status as an integer.
@return the user status
*/
quint8 ToxProfileQuery::statusInt() const
{
    return static_cast<quint8>(status());
}

/**
@brief Sets the local profile'S user status
@param status   the user status
*/
void ToxProfileQuery::setStatus(quint8 newValue)
{
    ToxProfilePrivate* p = ToxProfilePrivate::current();
    quint8 oldStatus = statusInt();
    if (p && newValue != oldStatus) {
        p->toxSet([&p, &newValue](Tox* tox) {
            ToxTypes::UserStatus c_status =
                    static_cast<ToxTypes::UserStatus>(newValue);
            tox_self_set_status(tox, ToxerPrivate::toTox(c_status));
            p->on_status_changed(newValue);
        });
    }
}

/**
@brief ToxFriendQuery constructor
@param[in] parent   the parent object
*/
ToxFriendQuery::ToxFriendQuery(QObject* parent)
    : QObject(parent)
{
}

/**
@brief Returns the number of friends for the active profile.
*/
int ToxFriendQuery::count() const
{
    const ToxProfilePrivate* p = ToxProfilePrivate::current();
    if (p) {
        return p->toxQuery([](const Tox* tox) -> QVariant
        {
            return static_cast<int>(tox_self_get_friend_list_size(tox));
        }).toInt();
    } else {
        return 0;
    }
}

/**
@brief Returns a list of friend id's for the active profile.
*/
QList<int> ToxFriendQuery::friends() const
{
    QList<int> l;
    const ToxProfilePrivate* p = ToxProfilePrivate::current();
    if (p) {
        l = p->toxQuery([](const Tox* tox) -> QVariant {
            const size_t cnt = tox_self_get_friend_list_size(tox);
            std::vector<uint32_t> ids(cnt);
            tox_self_get_friend_list(tox, ids.data());

            QList<int> out;
            for (uint32_t id : ids) {
                out << static_cast<int>(id);
            }

            return QVariant::fromValue(out);
        }).value<QList<int>>();
    }

    return l;
}

/**
@brief Add a Tox friend and send an accept request.
@param[in] pk   the friend's public key from the ToxID
@param[in] message  the "accept friend request" message
*/
void ToxFriendQuery::add(const QString& pk, const QString& message)
{
    Q_UNUSED(pk);
    Q_UNUSED(message);
    // TODO: implementation
}

/**
@brief Add a Tox friend without sending a request.
@param[in] pk   the friend's public key from the ToxID

Add a Tox friend without sending a request. This is useful for example when
importing a friend from another profile.
*/
void ToxFriendQuery::addNoRequest(const QString& pk)
{
    ToxProfilePrivate* p = ToxProfilePrivate::current();
    if (p) {
        p->toxSet([&](Tox* tox) {
            TOX_ERR_FRIEND_ADD err;
            uint8_t* c_pk = reinterpret_cast<uint8_t*>(pk.toLatin1().data());
            uint32_t c_index = tox_friend_add_norequest(tox, c_pk, &err);
            if (err) {
                qWarning("Could not add Tox friend %s: %s",
                         qUtf8Printable(pk),
                         ToxerPrivate::toxErrStr(err, ToxerPrivate::ToxContext::FriendAdd));
            } else {
                int index = static_cast<int>(c_index);
                p->on_friend_added(index);
            }
        });
    }
}

/**
@brief Remove a Tox friend from the profile.
@param index    the friend index
*/
void ToxFriendQuery::remove(int index)
{
    ToxProfilePrivate* p = ToxProfilePrivate::current();
    if (p) {
        p->toxSet([&](Tox* tox) {
            uint32_t c_index = static_cast<uint32_t>(index);
            TOX_ERR_FRIEND_DELETE err;
            tox_friend_delete(tox, c_index, &err);
            if (err) {
                qWarning("Could not remove Tox friend %i: %s", index,
                         ToxerPrivate::toxErrStr(err, ToxerPrivate::ToxContext::FriendDelete));
            } else {
                p->on_friend_deleted(index);
            }
        });
    }
}

/**
@brief Returns the public key of a friend.
@param[in] index    the friend index
*/
QString ToxFriendQuery::publicKeyStr(int index) const
{
    const ToxProfilePrivate* p = ToxProfilePrivate::current();
    if (p) {
        return p->toxQuery([&index](const Tox* tox) -> QVariant {
            return QString::fromLatin1(ToxerPrivate::pk(tox, index).toHex());
        }).toString();
    } else {
        return {};
    }
}

/**
@brief Returns the name of a friend.
@param[in] index    the friend index
@returns the friend's name; empty if index is invalid
*/
QString ToxFriendQuery::name(int index) const
{
    const ToxProfilePrivate* p = ToxProfilePrivate::current();
    if (p) {
        return p->toxQuery([&index](const Tox* tox) -> QVariant {
            const uint32_t c_index = static_cast<uint32_t>(index);
            TOX_ERR_FRIEND_QUERY err = TOX_ERR_FRIEND_QUERY_OK;
            size_t len = tox_friend_get_name_size(tox, c_index, nullptr);
            char* out = new char[len + 1];
            uint8_t* c_out = reinterpret_cast<uint8_t*>(out);

            tox_friend_get_name(tox, c_index, c_out, &err);
            out[len] = '\0';

            QString outStr = QString::fromUtf8(out);
            delete[] out;

            return outStr;
        }).toString();
    } else {
        return {};
    }
}

/**
@brief Returns the status message of a friend.
@param[in] index    the friend index
@returns the friend's status message; empty if index is invalid
*/
QString ToxFriendQuery::statusMessage(int index) const
{
    const ToxProfilePrivate* p = ToxProfilePrivate::current();
    if (p) {
        return p->toxQuery([&index](const Tox* tox) -> QVariant {
            const uint32_t c_index = static_cast<uint32_t>(index);
            TOX_ERR_FRIEND_QUERY err = TOX_ERR_FRIEND_QUERY_OK;
            size_t len = tox_friend_get_status_message_size(tox, c_index,
                                                            nullptr);
            char* out = new char[len + 1];
            uint8_t* c_out = reinterpret_cast<uint8_t*>(out);

            tox_friend_get_status_message(tox, c_index, c_out, &err);
            out[len] = '\0';

            QString outStr = QString::fromUtf8(out);
            delete[] out;

            return outStr;
        }).toString();
    } else {
        return {};
    }
}

/**
@brief Returns the online status of a friend.
@param index    the friend index
@return the online status
*/
bool ToxFriendQuery::isOnline(int index) const
{
    const ToxProfilePrivate* p = ToxProfilePrivate::current();
    if (p) {
        return p->toxQuery([&index](const Tox* tox) -> QVariant {
            const uint32_t c_index = static_cast<uint32_t>(index);
            TOX_ERR_FRIEND_QUERY err = TOX_ERR_FRIEND_QUERY_OK;
            TOX_CONNECTION connType =
                    tox_friend_get_connection_status(tox, c_index, &err);

            return connType != TOX_CONNECTION_NONE;
        }).toBool();
    } else {
        return false;
    }
}

/**
@brief Returns the friend's current user status.
@param[in] index    the friend index
@return the current user status
*/
ToxTypes::UserStatus ToxFriendQuery::status(int index) const
{
    const ToxProfilePrivate* p = ToxProfilePrivate::current();
    if (p) {
        QVariant t = p->toxQuery([&index](const Tox* tox) -> QVariant {
            const uint32_t c_index = static_cast<uint32_t>(index);
            TOX_ERR_FRIEND_QUERY err = TOX_ERR_FRIEND_QUERY_OK;
            TOX_USER_STATUS out = tox_friend_get_status(tox, c_index, &err);

            return ToxTypes::toQVariant(ToxerPrivate::fromTox(out));
        });

        return t.value<ToxTypes::UserStatus>();
    } else {
        return ToxTypes::UserStatus::Away;
    }
}

/**
@brief Returns the Tox status
@param index    the friend index
@return the currrent user status as integer
*/
quint8 ToxFriendQuery::statusInt(int index) const
{
    return static_cast<quint8>(status(index));
}

/**
@brief Returns the typing status.
@param index    the friend index
@return true, if the friend is typing; false otherwise
@note The typing status is only transferred, if the friend enables it.
*/
bool ToxFriendQuery::isTyping(int index) const
{
    const ToxProfilePrivate* p = ToxProfilePrivate::current();
    if (p) {
        return p->toxQuery([&index](const Tox* tox) {
            uint32_t c_index = static_cast<uint32_t>(index);
            TOX_ERR_FRIEND_QUERY err = TOX_ERR_FRIEND_QUERY_OK;
            return tox_friend_get_typing(tox, c_index, &err);
        }).toBool();
    } else {
        return false;
    }
}

/**
@brief friend added notifier
@param index    the friend index
*/
void ToxFriendQuery::on_added(int index)
{
    emit added(index);
    emit countChanged();
}

/**
@brief friend deleted notifier
@param index    the friend index
*/
void ToxFriendQuery::on_deleted(int index)
{
    emit removed(index);
    emit countChanged();
}

/**
@brief friend name changed notifier
@param index    the friend index
*/
void ToxFriendQuery::on_name_changed(int index, const QString& name)
{
    emit nameChanged(index, name);
}

/**
@brief friend status message changed notifier
@param index    the friend index
*/
void ToxFriendQuery::on_status_message_changed(int index, const QString& message)
{
    emit statusMessageChanged(index, message);
}

/**
@brief friend status changed notifier
@param index    the friend index
*/
void ToxFriendQuery::on_status_changed(int index, quint8 status)
{
    emit statusChanged(index, status);
}

/**
@brief friend is online changed notifier
@param index    the friend index
*/
void ToxFriendQuery::on_is_online_changed(int index, bool online)
{
    emit isOnlineChanged(index, online);
}

/**
@brief friend is typing notifier
@param index    the friend index
*/
void ToxFriendQuery::on_is_typing_changed(int index, bool typing)
{
    emit isTypingChanged(index, typing);
}

/**
@brief friend message received notifier
@param index    the friend index
*/
void ToxFriendQuery::on_message(int index, const QString& message)
{
    emit this->message(index, message);
}

/**
@brief ToxMessenger constructor
*/
ToxMessenger::ToxMessenger(QObject* parent)
    : ToxFriendQuery(parent)
{
}

/**
@brief Send a message to a friend or group.
@param[in] friendindex  the friend index of the receiver
@param[in] message  the message
*/
void ToxMessenger::sendMessage(int friendIndex, const QString& message)
{
    ToxProfilePrivate* p = ToxProfilePrivate::current();
    if (p) {
        p->toxSet([&friendIndex, &message](Tox* tox) {
            uint32_t c_index = static_cast<uint32_t>(friendIndex);
            QByteArray str = message.toUtf8();
            const uint8_t* c_str =
                    reinterpret_cast<const uint8_t*>(str.constData());
            size_t c_len = static_cast<size_t>(message.length());
            TOX_ERR_FRIEND_SEND_MESSAGE err = TOX_ERR_FRIEND_SEND_MESSAGE_OK;
            tox_friend_send_message(tox, c_index, TOX_MESSAGE_TYPE_NORMAL,
                                    c_str, c_len, &err);
            if (err != TOX_ERR_FRIEND_SEND_MESSAGE_OK) {
                qWarning("Sending message to friend %d failed: %s",
                         friendIndex, ToxerPrivate::toxErrStr(err));
            }
        });
    }
}
