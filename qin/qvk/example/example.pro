TEMPLATE = app
INCLUDEPATH += .

include(../qvirtualkeyboard.pri)

# Input

TARGET = UseKeyBoardDialog
HEADERS += UseKeyBoardDialog.h \		
			
FORMS += UseKeyBoardDialog.ui
SOURCES += Main.cpp \
		UseKeyBoardDialog.cpp
