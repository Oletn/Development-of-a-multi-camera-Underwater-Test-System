# Development-of-a-multi-camera-Underwater-Test-System

<p align="center">
  <img src="https://github.com/Oletn/Development-of-a-multi-camera-Underwater-Test-System/assets/148344905/5fdf5a3c-9f9e-477d-aab0-6cbdcd7547e7" alt="buoy" width="45%" />
  <img src="https://github.com/Oletn/Development-of-a-multi-camera-Underwater-Test-System/assets/148344905/700f405e-5a3f-485b-8ddc-4139eeb9b29c" alt="goedome" width="45%" />
</p>

This repo is part of a bachelor thesis in collaberation with the "Legacy of Dannevig: development of automated coastal monitoring" project by the Institute of Marine Research(IMR). The primary goal of the Legacy of Dannevig project is to develop a unit that is capable of real-time 3D reconstruction underwater. This thesis focuses on the development and design of the hardware necessary to achieve real-time 3D reconstruction in underwater environments. The hardware will be utilized in a doctoral project currently underway at the University of Agder (UiA).

Discussions with the supervisors, the IMR and testing Gaussian splatting technology during the concept development phase gave the requirements for the physical unit.The findings indicated that the frame needed to be rigid to keep the cameras stable while looking at the volume of water from all angles. A versatile and robust geodesic dome design was chosen and developed to meet these criteria. 

After testing Gaussian Splatting, the number of cameras needed was estimated to be between twenty to thirty. The NileCAM25 from e-con was chosen together with a Jetson Xavier processing units to handle the data gathered for the 3D reconstruction and AI. A camera housing was developed to fit the NileCAM25 inside the geodesic dome, but it can also be reused in future projects. The dome fits a total of twenty four cameras, with each camera placed inside a hub for environmental protection and sturdiness.

To ensure the rig is deployed in a suitable position without the need for divers or ROVs an inspection camera was developed. The inspection camera is set to be mounted at the top of the geodesic dome, looking down into the volume of water and sea bottom. It can be controlled remotely to check all angles inside the dome and it can be live streamed to most phones and computers by connecting to a WLAN and accessing a specific IP address in a web browser. The inspection camera is connected by a tethered power over ethernet cable to a smart buoy. The buoy has both WLAN and LoRa capabilities for wireless communication over long and short distances.

The inspection camera and the smart-buoy was built during the project period, whilst the geodesic dome was designed and some parts of it 3D printed. The inspection camera and buoy was successfully tested in the laboratory and the ocean. Additionally the geodesic dome was simulated in an virtual environment using Unreal Engine. The dome structure is set to be built during the summer 2024 at UiA. For the complete system to reach its full potential some development remains on the geodesic dome and smart buoy. 

Links: 

- [Gaussian Splatt of the built harware](https://poly.cam/capture/2C4342B9-2C42-4000-ADCE-34B9547F4CDA)
- [Gaussian Splatt attempt of table scene](https://poly.cam/capture/99aa364c-8df5-4117-9ccb-add820ba049c)
- [Gaussian Splatt attempt in unreal underwater environment](https://poly.cam/capture/67e4c845-c7d3-43b6-981c-8c3f783e6091)
- [Youtube Video of the project](https://youtu.be/iwjtGxDytKI)

