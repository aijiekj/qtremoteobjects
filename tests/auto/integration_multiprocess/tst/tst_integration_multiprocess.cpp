/****************************************************************************
**
** Copyright (C) 2014 Ford Motor Company
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtRemoteObjects module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL21$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** As a special exception, The Qt Company gives you certain additional
** rights. These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtTest/QtTest>
#include <QMetaType>
#include <QProcess>
#include <QStandardPaths>

namespace {

QString findExecutable(const QString &executableName, const QStringList &paths)
{
    const auto path = QStandardPaths::findExecutable(executableName, paths);
    if (!path.isEmpty()) {
        return path;
    }

    qWarning() << "Could not find executable:" << executableName << "in any of" << paths;
    return QString();
}

}

class tst_Integration_MultiProcess: public QObject
{
    Q_OBJECT

private slots:
    void initTestCase()
    {
        QLoggingCategory::setFilterRules("qt.remoteobjects.warning=false");
    }

    void cleanup()
    {
        // wait for delivery of RemoveObject events to the source
        QTest::qWait(200);
    }

    void testRun()
    {
        qDebug() << "Starting server process";
        QProcess serverProc;
        serverProc.setProcessChannelMode(QProcess::ForwardedChannels);
        serverProc.start(findExecutable("server", {
            QCoreApplication::applicationDirPath() + "/../server/"
        }));
        QVERIFY(serverProc.waitForStarted());

        // wait for server start
        QTest::qWait(200);

        qDebug() << "Starting client process";
        QProcess clientProc;
        clientProc.setProcessChannelMode(QProcess::ForwardedChannels);
        clientProc.start(findExecutable("client", {
            QCoreApplication::applicationDirPath() + "/../client/"
        }));
        QVERIFY(clientProc.waitForStarted());

        QVERIFY(clientProc.waitForFinished());
        QVERIFY(serverProc.waitForFinished());

        QCOMPARE(serverProc.exitCode(), 0);
        QCOMPARE(clientProc.exitCode(), 0);
    }
};

QTEST_MAIN(tst_Integration_MultiProcess)

#include "tst_integration_multiprocess.moc"
