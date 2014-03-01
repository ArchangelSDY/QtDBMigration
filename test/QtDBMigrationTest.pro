#-------------------------------------------------
#
# Project created by QtCreator 2014-03-01T18:01:04
#
#-------------------------------------------------

QT       += sql testlib

QT       -= gui

TARGET = QtDBMigrationTest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

include("../src/QtDBMigration.pri")

SOURCES += \
    QtDBMigrationTest.cpp

INCLUDEPATH += \
    ../src

DEFINES += SRCDIR=\\\"$$PWD/\\\"

assets.files = $$PWD/assets/*
assets.path = $$OUT_PWD

INSTALLS += assets
