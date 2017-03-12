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

#include <QGuiApplication>
#include <QQmlEngine>
#include <QQmlContext>
#include <QQuickView>
#include <QThread>

#include "Private/ToxProfile.h"
#include "Settings.h"
#include "Toxer.h"
#include "ToxTypes.h"

inline static void registerQmlTypes() {
    ToxTypes::registerQmlTypes();
    Settings::registerQmlTypes();

    constexpr const char* modComponents = { "com.tox.qmlcomponents" };
    qmlRegisterType<ToxProfileQuery>(modComponents, 1, 0, "ToxProfileQuery");
    qmlRegisterType<ToxFriendQuery>(modComponents, 1, 0, "ToxFriendQuery");
}

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    app.thread()->setObjectName(QStringLiteral("MainThread"));
    app.setOrganizationName(QStringLiteral("Tox"));
    app.setApplicationName(QStringLiteral("Toxer"));
    app.setApplicationVersion(QStringLiteral(TOXER_VERSION));

    registerQmlTypes();

    Toxer toxer;
    QQuickView view;
    view.setTitle(app.applicationDisplayName());
    view.rootContext()->setContextProperty(QStringLiteral("Toxer"), &toxer);
    view.setResizeMode(QQuickView::SizeRootObjectToView);

    view.setSource(Toxer::profileSelector());

    QObject::connect(&toxer, &Toxer::activeProfileChanged, [&view]()
    {
        const QUrl page = ToxProfilePrivate::current()
                          ? Toxer::mainView()
                          : Toxer::profileSelector();

        // important: let the event loop do the layout correctly
        QMetaObject::invokeMethod(&view, "setSource", Qt::QueuedConnection,
                                  Q_ARG(QUrl, page));
    });

    QObject::connect(&view, &QQuickView::statusChanged,
                     [&view](QQuickView::Status status) {
        if (status == QQuickView::Ready) {
            view.resize(view.initialSize());
        }
    });

    QQmlEngine* engine = view.engine();
    QObject::connect(engine, &QQmlEngine::quit, &app, &QGuiApplication::quit);

    view.show();

    return app.exec();
}
