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

#ifndef TOXER_PRIVATE_TOXPROFILE_H
#define TOXER_PRIVATE_TOXPROFILE_H

#include "ToxerPrivate.h"

#include <QMutex>
#include <QThread>
#include <QVector>

class IToxFriendNotifier;
class IToxProfileNotifier;

/**
@class ToxProfile::Private
@brief private implementation
*/
class ToxProfilePrivate final
{
    class ToxEventLoop final : public QThread
    {
        friend class ToxProfilePrivate;

    public:
        ToxEventLoop(Tox* _tox);
#if 0
        inline ~ToxEventLoop() override { qInfo("TEL destroyed"); }
#endif

        void bootstrap();
        inline void stop() {
            Q_ASSERT(QThread::currentThread() != this);
            QMutexLocker lock(&mutex_);
            active_ = false;
            quit(); // we don't actually use an event loop!
        }

    private:
        void run() final;

    private:
        mutable QMutex mutex_;
        Tox* tox_;
        bool active_;
    };

public:
    using Ptr = ToxProfilePrivate*;

public:
    inline static Ptr current() {
        return activeProfile;
    }

    static void create(const QString& name, const QString& password);
    static QByteArray load(const QString& name, const QString& password);
    static void activate(const QString& profileName,
                         const QString& password);

private:
    static Tox* createTox(const QByteArray& profileData);

public:
    using ToxFunc = std::function<QVariant (const Tox*)>;
    using ToxSetFunc = std::function<void (Tox*)>;

public:
    ToxProfilePrivate(const QString& _name, const QByteArray& profileData);
    ~ToxProfilePrivate();

    inline const QString name() const {
        return mName;
    }

    void start();

    QVariant toxQuery(ToxFunc query_func) const;
    void toxSet(ToxSetFunc set_func);

    void addNotificationObserver(IToxFriendNotifier* notify);
    void removeNotificationObserver(IToxFriendNotifier* notify);

    void addNotificationObserver(IToxProfileNotifier* notify);
    void removeNotificationObserver(IToxProfileNotifier* notify);

public:
    // profile notifiers
    void on_status_changed(int status);
    void on_status_message_changed(const QString& message);
    void on_user_name_changed(const QString& userName);

private:
    QString mName;
    ToxEventLoop* mTEL;

    QVector<IToxProfileNotifier*> profileNotifiers;
    QVector<IToxFriendNotifier*> friendNotifiers;

private:
    static Ptr activeProfile;
};

#endif
