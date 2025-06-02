# Third-Party Attributions

This project uses code from the following third-party sources. The use of this code is subject to the terms and conditions of their respective licenses. While the overall project is licensed under the GNU Affero General Public License v3 (AGPLv3), the specific components listed below retain their original licensing terms where applicable, and attribution is provided as required.

---

## Igneous Game Engine

- **Project:** Igneous
- **Copyright:** Copyright (c) MissingBitStudios
- **License:** Creative Commons Attribution 4.0 International (CC-BY-4.0)
- **License Text:** You can find the full license text at: [https://creativecommons.org/licenses/by/4.0/](https://creativecommons.org/licenses/by/4.0/)
- **Source Code:** The original source code is available at: [https://github.com/MissingBitStudios/igneous](https://github.com/MissingBitStudios/igneous)
- **Notice:** Portions of the Igneous engine code have been incorporated into this project and can be found within these files:
    - `src/mesh.hpp`: Took inspiration from the `model.hpp` file of Igneous.
    - `src/material.hpp`: Edited the namespace.

---

## Minecraft Again

- **Project:** minecraft-again
- **Copyright:** Copyright (c) jdah
- **License:** MIT License
- **Source Code:** The original source code is available at: [https://github.com/jdah/minecraft-again](https://github.com/jdah/minecraft-again)
- **License Grant & Notice:**
```text
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```
- **Usage Notice:** Portions of the Minecraft-again code have been incorporated into this project and can be found within these files:
  - `src/camera.cpp`: Added jdah's View Matrix generation code into this class.

---

## Chili Direct3D Engine

- **Project:** Chili Direct3D Engine
- **Copyright:** Copyright (c) 2018 PlanetChili <http://www.planetchili.net>
- **License:** [GNU General Public License v3 (GPLv3)](https://www.gnu.org/licenses/gpl-3.0)
- **Source Code:** The original source code is available at: [https://github.com/planetchili/hw3d](https://github.com/planetchili/hw3d)
- **License Grant & Notice:**
```cpp
/**************************************************************************************
* Chili Direct3D Engine																  *
* Copyright 2018 PlanetChili <http://www.planetchili.net>							  *
* 																					  *
* This file is part of Chili Direct3D Engine.										  *
* 																					  *
* Chili Direct3D Engine is free software: you can redistribute it and/or modify		  *
* it under the terms of the GNU General Public License as published by				  *
* the Free Software Foundation, either version 3 of the License, or					  *
* (at your option) any later version.												  *
* 																					  *
* The Chili Direct3D Engine is distributed in the hope that it will be useful,		  *
* but WITHOUT ANY WARRANTY; without even the implied warranty of					  *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
* GNU General Public License for more details.										  *
* 																					  *
* You should have received a copy of the GNU General Public License					  *
* along with The Chili Direct3D Engine.  If not, see <http://www.gnu.org/licenses/>.  *
**************************************************************************************/
```
- **Usage Notice:** Portions of the Chili Direct3D Engine code have been incorporated into this project and can be found within these files:
  - `src/camera.hpp`: Removed Direct3D specific code, replaced it with GLM's and edited the namespace.
  - `src/camera.cpp`: Removed Direct3D specific code, replaced it with GLM's and edited the namespace.
  - `src/model.hpp`: Removed Direct3D specific code, replaced it with GLM's and edited the namespace.
  - `src/model.cpp`: Removed Direct3D specific code, replaced it with GLM's and edited the namespace.

---


*This project, in its entirety (including contributions and incorporated code), is distributed under the terms of the GNU Affero General Public License v3 (AGPLv3), a copy of which is included as a `LICENSE` file.*
