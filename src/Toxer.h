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

#ifndef TOXER_H
#define TOXER_H

#include "ToxTypes.h"
#include "IToxNotify.h"

#include <QObject>
#include <QUrl>

class Toxer final : public QObject
{
    Q_OBJECT

public:
    inline static QUrl profileSelector()
    {
        return QUrl(QStringLiteral("qrc:/qml/Profiles.qml"));
    }

    inline static QUrl mainView()
    {
        return QUrl(QStringLiteral("qrc:/qml/MainView.qml"));
    }

public:
    Toxer();

    Q_INVOKABLE QString toxVersionString();

    Q_INVOKABLE QStringList availableProfiles() const;
    Q_INVOKABLE void activateProfile(const QString& profileName,
                                     const QString& password);

    Q_INVOKABLE QUrl avatarsUrl() const;
    Q_INVOKABLE bool exists(const QUrl& url) const;

signals:
    void activeProfileChanged();
};

class ToxProfileQuery : public QObject, IToxProfileNotifier
{
    Q_OBJECT

public:
    ToxProfileQuery(QObject* parent = nullptr);

    Q_INVOKABLE QString name() const;

    Q_INVOKABLE QString userName() const;
    Q_INVOKABLE void setUserName(const QString& newValue);
    Q_INVOKABLE QString statusMessage() const;
    Q_INVOKABLE void setStatusMessage(const QString& newValue);
    Q_INVOKABLE QString publicKeyStr() const;
    Q_INVOKABLE quint32 nospam() const;
    Q_INVOKABLE bool isOnline() const;
    ToxTypes::UserStatus status() const;
    Q_INVOKABLE quint8 statusInt() const;
    Q_INVOKABLE void setStatus(quint8 newValue);

signals:
    void userNameChanged(const QString& userName) final;
    void isOnlineChanged(bool online) final;
    void statusMessageChanged(const QString& statusMessage) final;
    void statusChanged(int status) final;
};

class ToxFriendQuery : public QObject, IToxFriendNotifier
{
    Q_OBJECT

public:
    ToxFriendQuery(QObject* parent = nullptr);
    ~ToxFriendQuery();

public:
    Q_INVOKABLE int count() const;
    Q_INVOKABLE QList<int> friends() const;

    Q_INVOKABLE QString publicKeyStr(int index) const;
    Q_INVOKABLE QString name(int index) const;
    Q_INVOKABLE QString statusMessage(int index) const;
    Q_INVOKABLE bool isOnline(int index) const;
    ToxTypes::UserStatus status(int index) const;
    Q_INVOKABLE quint8 statusInt(int index) const;
    Q_INVOKABLE bool isTyping(int index) const;

signals:
    void nameChanged(int index, const QString& name) final;
    void statusMessageChanged(int index, const QString& statusMessage) final;
    void statusChanged(int index, quint8 status) final;
    void isOnlineChanged(int index, bool online) final;
    void isTypingChanged(int index, bool typing) final;
    void message(int index, const QString& message) final;
};

#endif
