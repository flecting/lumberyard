/*
* All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
* its licensors.
*
* For complete copyright and license terms please see the LICENSE at the root of this
* distribution (the "License"). All use of this software is governed by the License,
* or, if provided, by the license below or the license accompanying this file. Do not
* remove or modify any license notices. This file is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*
*/
#pragma once

#include <AzCore/Component/Component.h>
#include <AzCore/IO/SystemFile.h>
#include <AzCore/IO/FileIO.h>

#include <Source/PythonCommon.h>
#include <Source/PythonUtility.h>
#include <Source/PythonSymbolsBus.h>

#include <EditorPythonBindings/EditorPythonBindingsBus.h>
#include <AzToolsFramework/API/EditorPythonConsoleBus.h>

namespace AZ
{
    class BehaviorClass;
    class BehaviorMethod;
    class BehaviorProperty;
}

namespace EditorPythonBindings
{
    //! Exports Python symbols to the log folder for Python script developers to include into their local projects
    class PythonLogSymbolsComponent
        : public AZ::Component
        , private EditorPythonBindingsNotificationBus::Handler
        , private PythonSymbolEventBus::Handler
        , private AzToolsFramework::EditorPythonConsoleInterface
    {
    public:
        AZ_COMPONENT(PythonLogSymbolsComponent, "{F1873D04-C472-41A2-8AA4-48B0CE4A5979}");

        static void Reflect(AZ::ReflectContext* context) {}

    protected:
        ////////////////////////////////////////////////////////////////////////
        // AZ::Component interface implementation
        void Activate() override;
        void Deactivate() override;

        ////////////////////////////////////////////////////////////////////////
        // EditorPythonBindingsNotificationBus::Handler
        void OnPostInitialize() override;

        ////////////////////////////////////////////////////////////////////////
        // PythonSymbolEventBus::Handler
        void LogClass(AZStd::string_view moduleName, AZ::BehaviorClass* behaviorClass) override;
        void LogClassMethod(AZStd::string_view moduleName, AZStd::string_view globalMethodName, AZ::BehaviorClass* behaviorClass, AZ::BehaviorMethod* behaviorMethod) override;
        void LogBus(AZStd::string_view moduleName, AZStd::string_view busName, AZ::BehaviorEBus* behaviorEBus) override;
        void LogGlobalMethod(AZStd::string_view moduleName, AZStd::string_view methodName, AZ::BehaviorMethod* behaviorMethod) override;
        void LogGlobalProperty(AZStd::string_view moduleName, AZStd::string_view propertyName, AZ::BehaviorProperty* behaviorProperty) override;
        void Finalize() override;

        ////////////////////////////////////////////////////////////////////////
        // EditorPythonConsoleInterface
        void GetModuleList(AZStd::vector<AZStd::string_view>& moduleList) const override;
        void GetGlobalFunctionList(GlobalFunctionCollection& globalFunctionCollection) const override;

    private:
        using ModuleSet = AZStd::unordered_set<AZStd::string>;
        using GlobalFunctionEntry = AZStd::pair<const AZ::BehaviorMethod*, AZStd::string>;
        using GlobalFunctionList = AZStd::vector<GlobalFunctionEntry>;
        using GlobalFunctionMap = AZStd::unordered_map<AZStd::string_view, GlobalFunctionList>;

        AZStd::string m_basePath;
        ModuleSet m_moduleSet;
        GlobalFunctionMap m_globalFunctionMap;

        AZ::IO::HandleType OpenInitFileAt(AZStd::string_view moduleName);
        AZ::IO::HandleType OpenModuleAt(AZStd::string_view moduleName);
        void WriteMethod(AZ::IO::HandleType handle, AZStd::string_view methodName, const AZ::BehaviorMethod& behaviorMethod, const AZ::BehaviorClass* behaviorClass);
        void WriteProperty(AZ::IO::HandleType handle, int level, AZStd::string_view propertyName, const AZ::BehaviorProperty& property, const AZ::BehaviorClass* behaviorClass);
    };
}
