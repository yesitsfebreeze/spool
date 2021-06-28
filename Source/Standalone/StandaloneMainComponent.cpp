#include "StandaloneWindow.h"
#include "StandaloneMainComponent.h"
#include "StandaloneWindow.h"
#include "StandaloneApp.h"
#include "../PluginEditor.h"

StandaloneMainComponent::StandaloneMainComponent(StandaloneWindow& window) : owner (window), editor (owner.getAudioProcessor()->hasEditor() ? owner.getAudioProcessor()->createEditorIfNeeded() : new juce::GenericAudioProcessorEditor (*owner.getAudioProcessor())) {
    

    if (editor != nullptr){
        editor->addComponentListener (this);
        componentMovedOrResized (*editor, false, true);
        addAndMakeVisible (editor.get());
#if JUCE_MAC
        juce::MenuBarModel::setMacMainMenu(this);
#endif
    }

    
//    juce::Value& inputMutedValue = owner.pluginHolder->getMuteInputValue();
    if (owner.pluginHolder->getProcessorHasPotentialFeedbackLoop())
    {
        // shouldShowNotification = inputMutedValue.getValue();
        //TODO: show feedback loop alert
    }
}

StandaloneMainComponent::~StandaloneMainComponent() {
    if (editor == nullptr) return;
#if JUCE_MAC
    MenuBarModel::setMacMainMenu(nullptr);
#endif
    editor->removeComponentListener(this);
    owner.pluginHolder->processor->editorBeingDeleted(editor.get());
    editor = nullptr;
}

void StandaloneMainComponent::resized() {
    auto r = getLocalBounds();
    if (editor == nullptr) return;
    
    editor->setBoundsConstrained(editor->getLocalArea(this, r.toFloat()).withPosition(r.getTopLeft().toFloat().transformedBy(editor->getTransform().inverted())).toNearestInt());
}


// menu
juce::StringArray StandaloneMainComponent::getMenuBarNames() {
    return menu.getMenuNames();
}

juce::PopupMenu StandaloneMainComponent::getMenuForIndex (int menuIndex, const juce::String& menuName) {
    juce::PopupMenu m;
    
    if (editor == nullptr) {};
    
    SpoolEditor* pluginEditor = (SpoolEditor*) editor.get();
    return menu.getMenuForIndex(pluginEditor->commandManager, m, menuIndex);
}

void StandaloneMainComponent::menuItemSelected (int itemID, int index) {
    // nothing to be done
}
// menu end


void StandaloneMainComponent::valueChanged (juce::Value& value) {
    //  inputMutedChanged (value.getValue());
    //TODO: show feedback loop alert
}


void StandaloneMainComponent::componentMovedOrResized (Component&, bool, bool) {
    if (editor != nullptr) {
        auto rect = getSizeToContainEditor();
        setSize (rect.getWidth(), rect.getHeight() + (shouldShowNotification ? StandaloneNotificationComponent::height : 0));
    }
}

juce::Rectangle<int> StandaloneMainComponent::getSizeToContainEditor() const {
    if (editor != nullptr) {
        return getLocalArea (editor.get(), editor->getLocalBounds());
    }

    return {};
}
