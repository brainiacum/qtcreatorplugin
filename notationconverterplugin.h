/*
* Copyright (c) 2017 Vitali Parkhomenko
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#pragma once

#include "notationconverter_global.h"
#include <extensionsystem/iplugin.h>
#include <functional>

namespace Core {
class IDocument;
class IEditor;
}
namespace TextEditor {class TextEditorWidget;}

namespace NotationConverter {
namespace Internal {

class NotationConverterPlugin : public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "NotationConverter.json")

public:
    NotationConverterPlugin();
    ~NotationConverterPlugin();

    bool initialize(const QStringList &arguments, QString *errorString);
    void extensionsInitialized();
    ShutdownFlag aboutToShutdown();

public slots:
    void convertPostfixToInfixCurrentFile();
    void convertPostfixToInfixSelectedText();
    void changedCurrentEditor(Core::IEditor *editor);

private:
    std::function<void(bool)> activateToolBarActions;
};

} // namespace Internal
} // namespace NotationConverter
