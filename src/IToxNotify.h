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

#ifndef TOXER_INTERFACE_TOX_NOTIFY_H
#define TOXER_INTERFACE_TOX_NOTIFY_H

#include <QString>

class ToxProfilePrivate;

class IToxFriendNotifier
{
protected:
    IToxFriendNotifier();
    virtual ~IToxFriendNotifier();

public:
    virtual void on_added(int index) = 0;
    virtual void on_deleted(int index) = 0;
    virtual void on_name_changed(int index, const QString& name) = 0;
    virtual void on_status_message_changed(int index, const QString& message) = 0;
    virtual void on_status_changed(int index, quint8 status) = 0;
    virtual void on_is_online_changed(int index, bool online) = 0;
    virtual void on_is_typing_changed(int index, bool typing) = 0;
    virtual void on_message(int index, const QString& message) = 0;
};

class IToxProfileNotifier
{
protected:
    IToxProfileNotifier();
    virtual ~IToxProfileNotifier();

public:
    virtual void on_user_name_changed(const QString& userName) = 0;
    virtual void on_is_online_changed(bool online) = 0;
    virtual void on_status_message_changed(const QString& message) = 0;
    virtual void on_status_changed(int status) = 0;
};

#endif
