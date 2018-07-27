#include "WMIBase/AsynNotifyQueryExtern.h"
#include "RecvEventNotify/InstanceEvent.h"
#include "SynEventQuery.h"
#include "WMIBase/AsynQueryExtern.h"
#include "SynQueryData.h"
#include "WMIBase/ExcuteMethod.h"

int main()
{
    //    setlocale(LC_CTYPE, "chs");
    HANDLE hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    CAsynNotifyQuery<CInstanceEvent> recvnotify(L"root\\CIMV2", L"SELECT * FROM __InstanceModificationEvent WITHIN 1 WHERE TargetInstance ISA 'Win32_Process'", hEvent);
    recvnotify.ExcuteFun();

//         CAsynNotifyQuery<CInstanceEvent> recvnotify(L"root\\CIMV2", L"SELECT * FROM __InstanceModificationEvent WITHIN 1 WHERE TargetInstance ISA 'Win32_Process' AND TargetInstance.Name ='system'", hEvent);
//         recvnotify.ExcuteFun();



    //    CAsynNotifyQuery<CInstanceEvent> recvnotify(L"root\\CIMV2", L"SELECT * FROM __InstanceCreationEvent WITHIN 1 WHERE TargetInstance ISA 'Win32_USBCOntrollerDevice'", hEvent);
    //    CAsynNotifyQuery<CInstanceEvent> recvnotify(L"root\\CIMV2",L"SELECT * FROM __InstanceDeletionEvent WITHIN 1 WHERE TargetInstance ISA 'Win32_USBCOntrollerDevice'", hEvent );

//         CAsynNotifyQuery<CInstanceEvent> recvnotify(L"root\\CIMV2",L"SELECT * FROM __InstanceCreationEvent WITHIN 1 WHERE TargetInstance ISA 'Win32_Process'", hEvent );
//         recvnotify.ExcuteFun();
    //    CAsynNotifyQuery<CInstanceEvent> recvnotify(L"root\\CIMV2",L"SELECT * FROM __InstanceDeletionEvent WITHIN 1 WHERE TargetInstance ISA 'Win32_Process'", hEvent );
    //    CAsynNotifyQuery<CInstanceEvent> recvnotify(L"root\\CIMV2",L"SELECT * FROM __InstanceModificationEvent WITHIN 1 WHERE TargetInstance ISA 'Win32_Process' And TargetInstance.Name = 'Notepad.exe'", hEvent );
    //    CAsynNotifyQuery<CInstanceEvent> recvnotify(L"root\\CIMV2", L"SELECT * FROM __InstanceModificationEvent WITHIN 1 WHERE TargetInstance ISA 'Win32_Process' And TargetInstance.ProcessId  = 13792", hEvent);

    //    CAsynNotifyQuery<CInstanceEvent> recvnotify(L"root\\CIMV2",L"SELECT * FROM __InstanceCreationEvent WITHIN 1 WHERE TargetInstance ISA 'Win32_Registry'", hEvent );
    //    CAsynNotifyQuery<CInstanceEvent> recvnotify(L"root\\CIMV2",L"SELECT * FROM __InstanceDeletionEvent WITHIN 1 WHERE TargetInstance ISA 'Win32_Registry'", hEvent );
    //    CAsynNotifyQuery<CInstanceEvent> recvnotify(L"root\\CIMV2", L"SELECT * FROM __InstanceModificationEvent WITHIN 1 WHERE TargetInstance ISA 'Win32_Registry'", hEvent);

    //    CAsynNotifyQuery<CInstanceEvent> recvnotify(L"root\\CIMV2", L"Select * From Win32_PingStatus Where Address=\"127.0.0.1\"", hEvent);

    //    CSynEventQuery recvnotify(L"root\\CIMV2", L"SELECT * FROM __InstanceCreationEvent WITHIN 1 WHERE TargetInstance ISA 'Win32_Process'");

    //    CAsynQuery<CInstanceEvent> recvnotify(L"root\\CIMV2",L"SELECT * FROM Win32_Process", hEvent );
    //    CSynQueryData recvnotify(L"root\\CIMV2",L"SELECT * FROM Win32_Process" );
    //    CSynQueryData recvnotify(L"root\\CIMV2",L"SELECT * FROM Win32_Processor" );

    //    CSynQueryData recvnotify(L"root\\CIMV2", L"Select * From Win32_PingStatus Where Address=\"192.0.0.2\"");

    //    CAsynNotifyQuery<CInstanceEvent> recvnotify(L"root\\CIMV2", L"SELECT * FROM  My_WMI_Perf_Counter WITHIN 1'", hEvent);

//     CExcuteMethod* excute_method = NULL;
//     {
//         ParamsMap params;
//         {
//             CComVariant vt;
//             vt.vt = VT_BSTR;
//             vt.bstrVal = CComBSTR("notepad.exe");
//             params[L"CommandLine"] = vt;
//         }
// 
//         excute_method = new CExcuteMethod(L"root\\CIMV2", L"Win32_Process", L"", L"Create", L"ReturnValue", params);
//         excute_method->ExcuteFun();
//     }
//     delete excute_method;

//     CSynQueryData recvnotify(L"root\\default", L"SELECT * FROM ClassInstance");//TestInstance
//     recvnotify.ExcuteFun();

//     CExcuteMethod* excute_method = NULL;
//     {
//         ParamsMap params;
//         {
//             CComVariant vt;
//             vt.vt = VT_BSTR;
//             vt.bstrVal = CComBSTR("notepad.exe");
//             params[L"CommandLine"] = vt;
//         }
// 
//         excute_method = new CExcuteMethod(L"root\\default", L"ClassInstance", L"func", L"ReturnValue", params);
//         excute_method->ExcuteFun();
//     }
//     delete excute_method;

//     CAsynNotifyQuery<CInstanceEvent> recvnotify(L"root\\default", L"SELECT * FROM ClassEventInstance", hEvent);
//     recvnotify.ExcuteFun();

//     CAsynNotifyQuery<CInstanceEvent> recvnotify(L"root\\default", L"SELECT * FROM __InstanceCreationEvent WITHIN 1 WHERE TargetInstance ISA 'IntrinsicClassInstance'", hEvent);
//     recvnotify.ExcuteFun();
    return 0;
}