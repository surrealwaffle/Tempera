/*
	Project: haloforge
	File: d3d9hook.cpp
	Copyright � 2009 SilentK, Abyll

	This file is part of haloforge.

    haloforge is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    haloforge is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with haloforge.  If not, see <http://www.gnu.org/licenses/>.

*/

#include <windows.h>
#include <fstream>
#include "../headers/detours.h"
#include "d3d9hook.h"
#include "textures.h"
#include "../headers/core.h"
#include "../headers/camera.h"

CD3D d3d;

CD3D::CD3D() {
	Font = NULL;
}

CD3D::~CD3D() {
}

long CD3D::GenerateShader(IDirect3DDevice9 *pDevice, IDirect3DPixelShader9 **pShader, float r, float g, float b) {
	char szShader[256];
	ID3DXBuffer *pShaderBuf = NULL;
	sprintf(szShader, "ps.1.1\ndef c0, %f, %f, %f, %f\nmov r0,c0", r, g, b, 1.0f);
	D3DXAssembleShader(szShader, sizeof(szShader), NULL, NULL, 0, &pShaderBuf, NULL);
	if (FAILED(pDevice->CreatePixelShader((const DWORD *) pShaderBuf->GetBufferPointer(), pShader)))return E_FAIL;
	return S_OK;
}

void CD3D::myDrawText(IDirect3DDevice9 *pDevice, ID3DXFont *pFont, bool shadow, long x, long y, long width, D3DCOLOR color, D3DCOLOR shadowcolor, const char *cFmt, ...) {
	va_list mvalist;
	char cBuffer[256] = {0};

	RECT Rect1 = {x + 1, y, x + 2000, y + 2000};
	RECT Rect2 = {x - 1, y, x + 2000, y + 2000};
	RECT Rect3 = {x, y + 1, x + 2000, y + 2000};
	RECT Rect4 = {x, y - 1, x + 2000, y + 2000};
	RECT Rect5 = {x, y, x + 2000, y + 2000};

		va_start(mvalist, cFmt);
	_vsnprintf(cBuffer, sizeof(cBuffer), cFmt, mvalist);
		va_end(mvalist);

	if (shadow == true) {
		pFont->DrawText(NULL, cBuffer, -1, &Rect1, 0, shadowcolor);
		pFont->DrawText(NULL, cBuffer, -1, &Rect2, 0, shadowcolor);
		pFont->DrawText(NULL, cBuffer, -1, &Rect3, 0, shadowcolor);
		pFont->DrawText(NULL, cBuffer, -1, &Rect4, 0, shadowcolor);
	}

	pFont->DrawText(NULL, cBuffer, -1, &Rect5, 0, color);
}

void CD3D::FillRGBA(ID3DXLine *pLine, float x, float y, float width, float height, D3DCOLOR color) {
	D3DXVECTOR2 vLine[2];
	pLine->SetWidth(width);
	pLine->SetAntialias(0);
	pLine->SetGLLines(1);
	vLine[0][0] = x + width / 2;
	vLine[0][1] = y;
	vLine[1][0] = x + width / 2;
	vLine[1][1] = y + height;
	pLine->Begin();
	pLine->Draw(vLine, 2, color);
	pLine->End();
}

void CD3D::DrawMouse(IDirect3DDevice9 *pDevice, IDirect3DTexture9 *pCursorTexture, ID3DXSprite *pCursorSprite) {
	POINT pPoint;
	float x, y;
	D3DCOLOR dColor = NULL;
	GetCursorPos(&pPoint);
	x = (float) pPoint.x;
	y = (float) pPoint.y;
	D3DXVECTOR3 Pos1;
	Pos1[0] = x;
	Pos1[1] = y;
	Pos1.z = 0.0f;
	pCursorSprite->Begin(D3DXSPRITE_ALPHABLEND);
	pCursorSprite->Draw(pCursorTexture, NULL, NULL, &Pos1, 0xFFFFFFFF);
	pCursorSprite->End();
}

typedef long (__stdcall *pEndScene)(IDirect3DDevice9 *pDevice);

pEndScene oEndScene;

void PrintObjectTags(IDirect3DDevice9 *pDevice) {
	Core *core = GetCore();

	object_data *obj = NULL;
	object_header *objh = NULL;
	object_header *temp_nearest = NULL;
	//CObjectArray* objA;
	//vect ObjCoord;
	const char *ObjName;
	vect3 screenpos;

	// for choosing the 'selected' object: (or the one closest to where we're aiming.
	float closest = .35f; // it needs to be at least within 30 degrees
	float tx;
	float ty;

	for (unsigned short i = 0; i < core->core_1->Object->max; i++) {
		objh = core->GetObjectHeader(i);
		obj = core->GetGenericObject(i);
		if (obj == NULL) {
			continue;
		}

		screenpos = core->MyCamera->ScreenPos(obj->World);
		ObjName = core->GetObjectName(i);

		tx = screenpos.x * 2 - 1;
		ty = screenpos.y * 2 - 1;
		tx = sqrt(tx * tx + ty * ty);
		if (tx < closest) {
			closest = tx;
			temp_nearest = objh;
		}
		// Offscreen check
		//Parens b/c it un-confuses CLion lol
		if ((screenpos.x < (-FOV_XBOUND)) || screenpos.x > (1 + FOV_XBOUND) || (screenpos.y < (-FOV_YBOUND)) || screenpos.y > (1 + FOV_YBOUND) ||
			screenpos.z > OBJECT_CLIP) {
			continue;
		}

		screenpos.x *= d3d.pViewport.Width;
		screenpos.y *= d3d.pViewport.Height;

		D3DCOLOR color = tGreen;
		if (objh == core->ObjectControl->selected_h)
			color = tOrange;
		else if (objh == core->ObjectControl->nearest_h)
			color = tBlue;
		d3d.myDrawText(pDevice, d3d.Font, true, (int) screenpos.x, (int) screenpos.y, 2000, color, tBlack, ObjName);
	}

	if (temp_nearest != NULL) {
		if(core->ObjectControl->nearest_h != temp_nearest) {
			core->ConsoleText(hGreen, "Setting object to nearest!");
			core->ObjectControl->nearest_h = temp_nearest;
		}
	}
}

long __stdcall hkEndScene(IDirect3DDevice9 *pDevice) {
	if (!d3d.Font) {
		D3DXCreateFont(pDevice, 15, 0, FW_BOLD, 1, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Ariel", &d3d.Font);
	}

	pDevice->GetViewport(&d3d.pViewport);

	PrintObjectTags(pDevice);

	return oEndScene(pDevice);
}

typedef long (__stdcall *pReset)(IDirect3DDevice9 *pDevice, D3DPRESENT_PARAMETERS *pPresentationParameters);

pReset oReset;

// Note: Xfire also hooks Reset, which means the game will crash if device is lost and reset while xfire is open

long __stdcall hkReset(IDirect3DDevice9 *pDevice, D3DPRESENT_PARAMETERS *pPresentationParameters) {

	if (d3d.Font)
		d3d.Font->OnLostDevice();

	HRESULT hResult = oReset(pDevice, pPresentationParameters);

	if (d3d.Font)
		d3d.Font->OnResetDevice();

	return hResult;
}

typedef long (__stdcall *pDrawIndexedPrimitive)(IDirect3DDevice9 *pDevice, D3DPRIMITIVETYPE PrimType, int BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT primCount);

pDrawIndexedPrimitive oDrawIndexedPrimitive;

long __stdcall hkDrawIndexedPrimitive(IDirect3DDevice9 *pDevice, D3DPRIMITIVETYPE PrimType, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT primCount) {
	__asm pushad

	__asm popad

	return oDrawIndexedPrimitive(pDevice, PrimType, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
}

typedef long (__stdcall *pSetStreamSource)(IDirect3DDevice9 *pDevice, UINT StreamNumber, IDirect3DVertexBuffer9 *pStreamData, UINT OffsetInBytes, UINT Stride);

pSetStreamSource oSetStreamSource;

long __stdcall hkSetStreamSource(IDirect3DDevice9 *pDevice, UINT StreamNumber, IDirect3DVertexBuffer9 *pStreamData, UINT OffsetInBytes, UINT Stride) {
	__asm pushad

	__asm popad

	return oSetStreamSource(pDevice, StreamNumber, pStreamData, OffsetInBytes, Stride);
}

// This function grabs the device pointer from Halo
// Then I assign the pointer to the vTable structure
// Then you can use DetourFunction to detour any of the functions
//    in the vTable

// vTable is just a huge table of pointers to every d3d9 function

DWORD __stdcall CD3D::hkD3DHook(void *lpVoid) {
	vTable_D3D9 *vD3D9; // Create instance of d3d9 Virtual Method Table

	//1.08 d3d device global 0x0071D09C
	//0x3C471C0
	//0x3C49F5C
	//0x6B840C
	void *pDevicePointer = (void *) 0x6B840C; // Halo full version device pointer ( static )
	DWORD dwOldProtect = NULL;
	IDirect3DDevice9 *pGameDevice;

	VirtualProtect((void *) pDevicePointer, 4, PAGE_EXECUTE_READWRITE, &dwOldProtect);
	memcpy(&pGameDevice, (void *) pDevicePointer, 4);
	VirtualProtect((void *) pDevicePointer, 4, dwOldProtect, NULL);

	DWORD *pHaloDevice = (DWORD *) pGameDevice;
	pHaloDevice = (DWORD *) pHaloDevice[0];
	vD3D9 = (vTable_D3D9 *) pHaloDevice; // Assign device table to our vTable structure

	oEndScene = (pEndScene) DetourFunction((PBYTE) vD3D9->EndScene, (PBYTE) hkEndScene);
	oReset = (pReset) DetourFunction((PBYTE) vD3D9->Reset, (PBYTE) hkReset);
	//oDrawIndexedPrimitive = (pDrawIndexedPrimitive)DetourFunction((PBYTE)vD3D9->DrawIndexedPrimitive, (PBYTE)hkDrawIndexedPrimitive);
	//oSetStreamSource = (pSetStreamSource)DetourFunction((PBYTE)vD3D9->SetStreamSource, (PBYTE)hkSetStreamSource);

	return 0;
}