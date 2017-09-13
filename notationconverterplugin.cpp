/*
* Copyright (c) 2017 Vitali Parkhomenko
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#include "notationconverterplugin.h"
#include "notationconverterconstants.h"
#include "notationconvertercore/notationconvertercore.h"

#include <coreplugin/actionmanager/command.h>
#include <coreplugin/actionmanager/actionmanager.h>
#include <coreplugin/actionmanager/actioncontainer.h>
#include <coreplugin/editormanager/editormanager.h>
#include <coreplugin/coreconstants.h>
#include <texteditor/textdocument.h>
#include <texteditor/texteditor.h>
#include <texteditor/texteditorconstants.h>
#include <utils/macroexpander.h>
#include <utils/qtcassert.h>

#include <QAction>
#include <QMenu>
#include <QTextBlock>

using namespace Core;
using namespace TextEditor;

namespace NotationConverter {
namespace Internal {

static NotationConverterPlugin *m_instance = 0;

NotationConverterPlugin::NotationConverterPlugin()
{
    QTC_ASSERT(!m_instance, return );
    m_instance = this;
}

NotationConverterPlugin::~NotationConverterPlugin()
{
    m_instance = 0;
}

bool NotationConverterPlugin::initialize(const QStringList& arguments, QString *errorString)
{
    Q_UNUSED(arguments)
    Q_UNUSED(errorString)

    m_postfixToInfix = new PostfixToInfix;
    m_postfixToInfix->customOperations = {
        {QString("!"), Operation{1, QString("%1 !"), false, QString("Factorial")}},
        {QString("ln"), Operation{1, QString("ln ( %1 )"), false, QString("Natural logarithm")}},
        {QString("log"), Operation{2, QString("log %1 ( %2 )"), false, QString("Logarithm")}},
        {QString("sin"), Operation{1, QString("sin ( %1 )"), false, QString("Sine")}},
        {QString("cos"), Operation{1, QString("cos ( %1 )"), false, QString("Cosine")}},
        {QString("tan"), Operation{1, QString("tan ( %1 )"), false, QString("Tangent")}},
        {QString("atan"), Operation{1, QString("atan ( %1 )"), false, QString("Arc tangent")}}
    };

    connect(EditorManager::instance(), &EditorManager::currentEditorChanged,
            this, &NotationConverterPlugin::changedCurrentEditor);

    // For holding toolbar actions NotationConverter for manipulate them
    QVector<QAction *> toolbarActions;

    // Create the Notation converter menu
    ActionContainer *subMenu = ActionManager::createMenu(Constants::MENU_POSTFIX_TO_INFIX_ID);
    subMenu->menu()->setTitle(tr("Postfix to Infix"));

    // Action with converting Posfix to Infix the current file
    auto convertCurrentFile = new QAction(tr("Current file"), this);
    Command *cmd = ActionManager::registerAction(convertCurrentFile,
                                                 Constants::POSTFIX_TO_INFIX_CURRENT_FILE_ID,
                                                 Context(Core::Constants::C_EDIT_MODE));
    cmd->setDefaultKeySequence(QKeySequence(tr("Ctrl+Meta+A")));
    connect(convertCurrentFile, &QAction::triggered,
            this, &NotationConverterPlugin::convertPostfixToInfixCurrentFile);
    subMenu->addAction(cmd);
    toolbarActions.append(convertCurrentFile);

    // Action with converting Posfix to Infix the selected text
    auto convertSelectedText = new QAction(tr("Selected text"), this);
    cmd = ActionManager::registerAction(convertSelectedText,
                                        Constants::POSTFIX_TO_INFIX_SELECTED_TEXT_ID,
                                        Context(Core::Constants::C_EDIT_MODE));
    cmd->setDefaultKeySequence(QKeySequence(tr("Ctrl+Meta+S")));
    connect(convertSelectedText, &QAction::triggered,
            this, &NotationConverterPlugin::convertPostfixToInfixSelectedText);
    subMenu->addAction(cmd);
    toolbarActions.append(convertSelectedText);

    m_setEnableConversion = [toolbarActions](bool enable) -> void {
        auto end = toolbarActions.end();
        for (auto it = toolbarActions.begin(); it != end; ++it)
            (*it)->setEnabled(enable);
    };

    // Deactivate all actions for default
    m_setEnableConversion(false);

    // Filling main menu Notation converter
    ActionContainer *mainMenu = ActionManager::createMenu(Constants::MENU_NOTATION_CONVERTER_ID);
    mainMenu->menu()->setTitle(tr("Notation Converter"));
    mainMenu->addMenu(subMenu);
    ActionManager::actionContainer(Core::Constants::M_EDIT_ADVANCED)->addMenu(mainMenu);

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
    // Works only with active BaseTextEditor
    BaseTextEditor *editor = qobject_cast<BaseTextEditor *>(EditorManager::currentEditor());
    if (editor == 0)
        return;

    QTextCursor cursor = editor->textCursor();
    cursor.setPosition(QTextCursor::Start, QTextCursor::MoveAnchor);
    cursor.beginEditBlock();
    QTextBlock textBlock = cursor.block();
    for (; textBlock.isValid(); textBlock = textBlock.next()) {
        QString textLine = textBlock.text();
        if (m_postfixToInfix->convert(textLine) == ConversionResult::SuccessConversion) {
            cursor.setPosition(textBlock.position(), QTextCursor::MoveAnchor);
            cursor.select(QTextCursor::LineUnderCursor);
            cursor.insertText(textLine);
        }
    }
    cursor.endEditBlock();
}

void NotationConverterPlugin::convertPostfixToInfixSelectedText()
{
    // Works only with active BaseTextEditor
    BaseTextEditor *editor = qobject_cast<BaseTextEditor *>(EditorManager::currentEditor());
    if (editor == 0)
        return;

    QTextCursor cursor = editor->textCursor();
    if (!cursor.hasSelection())
        return;

    // Getting users selected blocks
    cursor.setPosition(cursor.selectionEnd(), QTextCursor::KeepAnchor);
    QTextBlock const endBlock = cursor.block();
    cursor.setPosition(cursor.selectionStart(), QTextCursor::KeepAnchor);
    QTextBlock const startBlock = cursor.block();

    // Converting a selected text in the current line
    if (startBlock == endBlock) {
        QString textLine = cursor.selectedText();
        if (m_postfixToInfix->convert(textLine) == ConversionResult::SuccessConversion)
            cursor.insertText(textLine);
        return;
    }

    // Converting a selected text in the several lines
    cursor.beginEditBlock();
    QTextBlock textBlock = cursor.block();
    for (; textBlock.isValid() && !(endBlock < textBlock); textBlock = textBlock.next()) {
        cursor.setPosition(textBlock.position(), QTextCursor::MoveAnchor);
        cursor.select(QTextCursor::LineUnderCursor);
        QString textLine = cursor.selectedText();
        if (m_postfixToInfix->convert(textLine) == ConversionResult::SuccessConversion)
            cursor.insertText(textLine);
    }
    cursor.endEditBlock();
}

void NotationConverterPlugin::changedCurrentEditor(Core::IEditor *editor)
{
    // Activate toolbar actions only for active BaseTextEditor
    m_setEnableConversion(qobject_cast<BaseTextEditor *>(editor) != 0);
}

} // namespace Internal
} // namespace NotationConverter
