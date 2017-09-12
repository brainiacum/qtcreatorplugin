/*
* Copyright (c) 2017 Vitali Parkhomenko
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include "notationconverterplugin.h"
#include "notationconverterconstants.h"
#include "notationconvertercore/notationconvertercore.h"

#include <coreplugin/coreconstants.h>
#include <coreplugin/actionmanager/command.h>
#include <coreplugin/actionmanager/actionmanager.h>
#include <coreplugin/actionmanager/actioncontainer.h>
#include <coreplugin/editormanager/editormanager.h>

#include <texteditor/textdocument.h>
#include <texteditor/texteditor.h>
#include <texteditor/texteditorconstants.h>

#include <utils/macroexpander.h>
#include <utils/qtcassert.h>

#include <QMenu>
#include <QAction>
#include <QMainWindow>
#include <QTextBlock>

namespace NotationConverter {
namespace Internal {

using NotationConverter::PostfixToInfix;
using NotationConverter::Operation;
PostfixToInfix postfix2infix;

static NotationConverterPlugin* m_instance = 0;

NotationConverterPlugin::NotationConverterPlugin()
{
    QTC_ASSERT(!m_instance, return );
    m_instance = this;

    // Basic custom operations
    postfix2infix.customOperations = {
        {"!", Operation{1, QString("%1 !"), false, QString("Factorial")}},
        {"ln", Operation{1, QString("ln ( %1 )"), false, QString("Natural logarithm")}},
        {"log", Operation{2, QString("log %1 ( %2 )"), false, QString("Logarithm")}},
        {"sin", Operation{1, QString("sin ( %1 )"), false, QString("Sine")}},
        {"cos", Operation{1, QString("cos ( %1 )"), false, QString("Cosine")}},
        {"tan", Operation{1, QString("tan ( %1 )"), false, QString("Tangent")}},
        {"atan", Operation{1, QString("atan ( %1 )"), false, QString("Arc tangent")}},
        {"atan2", Operation{2, QString("cotan ( %1 , %2 )"), false, QString("Arc tangent, using signs to determine quadrants")}}};
}

NotationConverterPlugin::~NotationConverterPlugin()
{
    m_instance = 0;
}

bool NotationConverterPlugin::initialize(const QStringList& arguments, QString* errorString)
{
    Q_UNUSED(arguments)
    Q_UNUSED(errorString)

    connect(Core::EditorManager::instance(), &Core::EditorManager::currentEditorChanged,
            this, &NotationConverterPlugin::changedCurrentEditor);

    // Create the Notation converter menu
    Core::ActionContainer* menuPostixToInfix = Core::ActionManager::createMenu(Constants::MENU_POSTFIX_TO_INFIX_ID);
    menuPostixToInfix->menu()->setTitle(tr("Postfix to Infix"));

    // Action with converting Posfix to Infix the current file
    auto postfixToInfixCurrentFile = new QAction(tr("Current file"), this);
    Core::Command* cmd = Core::ActionManager::registerAction(postfixToInfixCurrentFile,
                                                             Constants::POSTFIX_TO_INFIX_CURRENT_FILE_ID,
                                                             Core::Context(Core::Constants::C_EDIT_MODE));
    cmd->setDefaultKeySequence(QKeySequence(tr("Ctrl+Meta+A")));
    connect(postfixToInfixCurrentFile, &QAction::triggered,
            this, &NotationConverterPlugin::convertPostfixToInfixCurrentFile);
    menuPostixToInfix->addAction(cmd);

    // Action with converting Posfix to Infix the selected text
    auto postfixToInfixSelectedText = new QAction(tr("Selected text"), this);
    cmd = Core::ActionManager::registerAction(postfixToInfixSelectedText,
                                              Constants::POSTFIX_TO_INFIX_SELECTED_TEXT_ID,
                                              Core::Context(Core::Constants::C_EDIT_MODE));
    cmd->setDefaultKeySequence(QKeySequence(tr("Ctrl+Meta+S")));
    connect(postfixToInfixSelectedText, &QAction::triggered,
            this, &NotationConverterPlugin::convertPostfixToInfixSelectedText);
    menuPostixToInfix->addAction(cmd);

    activateToolBarActions =
            [postfixToInfixCurrentFile,postfixToInfixSelectedText] (bool enable) -> void
    {
        postfixToInfixCurrentFile->setEnabled(enable);
        postfixToInfixSelectedText->setEnabled(enable);
    };

    // Deactivate all actions for default
    postfixToInfixCurrentFile->setEnabled(false);
    postfixToInfixSelectedText->setEnabled(false);

    // Filling main menu Notation converter
    Core::ActionContainer* mainMenu = Core::ActionManager::createMenu(Constants::MENU_NOTATION_CONVERTER_ID);
    mainMenu->menu()->setTitle(tr("Notation Converter"));
    mainMenu->addMenu(menuPostixToInfix);

    Core::ActionManager::actionContainer(Core::Constants::M_EDIT_ADVANCED)->addMenu(mainMenu);
    return true;
}

void NotationConverterPlugin::extensionsInitialized() { }

ExtensionSystem::IPlugin::ShutdownFlag NotationConverterPlugin::aboutToShutdown()
{
    disconnect(Core::EditorManager::instance(), &Core::EditorManager::currentEditorChanged,
               this, &NotationConverterPlugin::changedCurrentEditor);
    return SynchronousShutdown;
}

void NotationConverterPlugin::convertPostfixToInfixCurrentFile()
{
    using namespace Core;
    using namespace TextEditor;

    BaseTextEditor* editor = qobject_cast<BaseTextEditor*>(EditorManager::currentEditor());
    if (editor == 0)
        return;

    QTextCursor cursor = editor->textCursor();
    cursor.setPosition(QTextCursor::Start, QTextCursor::MoveAnchor);
    cursor.beginEditBlock();
    for (QTextBlock textBlock = cursor.block(); textBlock.isValid(); textBlock = textBlock.next()) {
        QString textLine = textBlock.text();
        ConversionResult res = postfix2infix.convert(textLine);
        if (res == ConversionResult::SuccessConversion) {
            cursor.setPosition(textBlock.position(), QTextCursor::MoveAnchor);
            cursor.select(QTextCursor::LineUnderCursor);
            cursor.insertText(textLine);
        }
    }
    cursor.endEditBlock();
}

void NotationConverterPlugin::convertPostfixToInfixSelectedText()
{
    using namespace Core;
    using namespace TextEditor;

    BaseTextEditor* editor = qobject_cast<BaseTextEditor*>(EditorManager::currentEditor());
    if (editor == 0)
        return;

    QTextCursor cursor = editor->textCursor();
    if (!cursor.hasSelection())
        return;

    cursor.setPosition(cursor.selectionEnd(), QTextCursor::KeepAnchor);
    QTextBlock const endBlock = cursor.block();
    cursor.setPosition(cursor.selectionStart(), QTextCursor::KeepAnchor);
    QTextBlock const startBlock = cursor.block();

    // Converting a selected text in the current line
    if (startBlock==endBlock) {
        QString textLine = cursor.selectedText();
        ConversionResult res = postfix2infix.convert(textLine);
        if (res == ConversionResult::SuccessConversion) {
            cursor.insertText(textLine);
        }
        return;
    }

    // Converting a selected text in the several lines
    cursor.beginEditBlock();
    for (QTextBlock textBlock = cursor.block(); textBlock.isValid() && !(endBlock < textBlock); textBlock = textBlock.next()) {
        cursor.setPosition(textBlock.position(), QTextCursor::MoveAnchor);
        cursor.select(QTextCursor::LineUnderCursor);
        QString textLine = cursor.selectedText();
        ConversionResult res = postfix2infix.convert(textLine);
        if (res == ConversionResult::SuccessConversion) {
            cursor.insertText(textLine);
        }
    }
    cursor.endEditBlock();
}

void NotationConverterPlugin::changedCurrentEditor(Core::IEditor *editor)
{
    using namespace Core;
    using namespace TextEditor;
    BaseTextEditor* btEditor = qobject_cast<BaseTextEditor*>(editor);
    if (btEditor!=0) {
        activateToolBarActions(true);
    } else {
        activateToolBarActions(false);
    }
}

} // namespace Internal
} // namespace NotationConverter
