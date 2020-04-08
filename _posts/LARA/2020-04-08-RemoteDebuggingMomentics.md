---
layout: post
title: "Remote Debugging "
date: 2020-04-08 01:00:00 +0800
categories: QNX Mometics Remote Debugging
---

# Remote Debugging(QNX)
---
## GBD를 이용한 Remote Debugging
* [GDB Remote Debugging](http://collab.lge.com/main/display/AUTOTECH/How+to+use+remote+GDB) : 장진왕S(19.1)

## QNX Mometics를 이용한 Remote Debugging

### 설치 필요 SW
* VMware([Download](https://www.vmware.com/kr/products/workstation-player/workstation-player-evaluation.html)) - QNX OS
* QNX Mometics(eclipse 기반 IDE) [Download](http://www.qnx.com/download/group.html?programid=29178)

### Mometics - Target(VMware) 연결
* Host(local PC) - Target(VMware ICAS3) IP connection
![RemoteDebugging_IPConnection.png](/assets/RemoteDebugging_IPConnection.png)

### Remote Debugging 방법
#### 1. Launching an application with the debugger attached

##### QNX Mometics에서 debugger를 추가햐여 Application(eg. speed_app)을 실행
1. Make New Project
![RemoteDebugging_newProject.png](/assets/RemoteDebugging_newProject.png)

2. Make Debug Configuration
* Make New_configuration
    * Main tab
    ![RemoteDebugging_DebugConfig_1.png](/assets/RemoteDebugging_DebugConfig_1.png)
    * Argumemts tab
        * parameter 가 필요한 경우  아래의 그림과 같이 설정
    ![RemoteDebugging_DebugConfig_2.png](/assets/RemoteDebugging_DebugConfig_2.png)
    * Source tab
    ![RemoteDebugging_DebugConfig_3.png](/assets/RemoteDebugging_DebugConfig_3.png)

* Run Debug
    * 아래의 그림과 같이 break Point가 없이 실행 될 경우, main에서 break 되어 있습니다.
    ![RemoteDebugging_DebugConfig_4.png](/assets/RemoteDebugging_DebugConfig_4.png)
    * Target(VMware ICAS3)에서 *$ pidin ar*을 통해 정상적으로 process 실행 됐는지 확인 가능합니다.
    ![RemoteDebugging_pidin.png](/assets/RemoteDebugging_pidin.png)

3. Add BreakPoint
* 아래의 그림과 같이 break point를 추가하면 원하는 breakpoint에서 variable을 확인 할 수 있습니다.
![RemoteDebugging_AddBreakPoint.png](/assets/RemoteDebugging_AddBreakPoint.png)

#### 2. Attaching the debugger to a running process
* Target에서 running 중인 process를 디버깅 하는 방법
    * 위의 방법과 거의 동일하나 현재 libm.so.4 등 맞지 않는 lib으로 인해 동작하기 어려움


### QNX Momentics를 이용한 Debugging 장점
* GBD를 이용하는 것은 동일
* UI를 이용하여 직관적이고 Vairable을 바로 확인 할 수 있음
* 추후 Profiling 및 codecoverage에도 비슷한 방식으로 확장 가능