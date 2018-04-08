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
    static QUrl profileSelector();
    static QUrl mainView();

public:
    Toxer();

    Q_INVOKABLE QString toxVersionString();

    Q_INVOKABLE QStringList availableProfiles() const;
    Q_INVOKABLE void activateProfile(const QString& profileName,
                                     const QString& password);
    Q_INVOKABLE void createProfile(const QString& profileName,
                                   const QString& password);

    Q_INVOKABLE QUrl avatarsUrl() const;
    Q_INVOKABLE bool exists(const QUrl& url) const;

signals:
    void reloadUi();
};

class ToxProfileQuery : public QObject, IToxProfileNotifier
{
    Q_OBJECT
public:
    ToxProfileQuery(QObject* parent = nullptr);

public:
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
    void userNameChanged(const QString& userName);
    void isOnlineChanged(bool online);
    void statusMessageChanged(const QString& statusMessage);
    void statusChanged(int status);

private:
    // IToxProfileNotifier interface
    void on_user_name_changed(const QString& userName) override;
    void on_is_online_changed(bool online) override;
    void on_status_message_changed(const QString& message) override;
    void on_status_changed(int status) override;
};

class ToxFriendQuery : public QObject, IToxFriendNotifier
{
    Q_OBJECT
public:
    ToxFriendQuery(QObject* parent = nullptr);

public:

    Q_INVOKABLE QList<int> friends() const;

    Q_INVOKABLE QString publicKeyStr(int index) const;
    Q_INVOKABLE QString name(int index) const;
    Q_INVOKABLE QString statusMessage(int index) const;
    Q_INVOKABLE bool isOnline(int index) const;
    ToxTypes::UserStatus status(int index) const;
    Q_INVOKABLE quint8 statusInt(int index) const;
    Q_INVOKABLE bool isTyping(int index) const;

signals:
    void countChanged();
    void added(int index);
    void removed(int index);
    void nameChanged(int index, const QString& name);
    void statusMessageChanged(int index, const QString& statusMessage);
    void statusChanged(int index, quint8 status);
    void isOnlineChanged(int index, bool online);
    void isTypingChanged(int index, bool typing);
    void message(int index, const QString& message);

private:
    // IToxFriendNotifier interface
    void on_added(int index) override;
    void on_deleted(int index) override;
    void on_name_changed(int index, const QString& name) override;
    void on_status_message_changed(int index, const QString& message) override;
    void on_status_changed(int index, quint8 status) override;
    void on_is_online_changed(int index, bool online) override;
    void on_is_typing_changed(int index, bool typing) override;
    void on_message(int index, const QString& message) override;
};

class ToxMessenger : public ToxFriendQuery
{
    Q_OBJECT
public:
    ToxMessenger(QObject* parent = nullptr);

public:
    Q_INVOKABLE void sendMessage(int friendIndex, const QString& message);
};

#endif
