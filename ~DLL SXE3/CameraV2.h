#ifndef CAMERAV2_H
#define CAMERAV2_H

//0106CB58   0000             ADD BYTE PTR DS:[EAX],AL

//004DFC5C   E8 B12E0000      CALL main.004E2B12
//005B6CFD   55               PUSH EBP

enum ObjState
{
	SelectServer	= 2,
	SwitchCharacter = 4,
	GameProcess		= 5,
};

#define pPlayerState			*(int*)0x12A5390 // SXE3
#define	pCamThis				((LPVOID(__cdecl*)()) 0x004E2A8D) // Location 1:1 function format NOT 1:1 SXE3
#define pGetCameraZoom			((float(__thiscall*)(LPVOID This)) 0x005C599E) // S9E2 Location not 1:1 Format 1:1 SXE3
#define	pCursorX				*(int*)0x8C84DAC // SXE3
#define	pCursorY				*(int*)0x8C84DB0 // SXE3 

#define oCam_Zoom				0x11010B8	// SXE3
#define oCam_PosZDef			0x10FCC00	// SXE3 Restore PosZ 0106CB58
#define oCam_PosZ				0x10FCC0C	// SXE3 PosZ
#define oCam_RotZ				0x8C84D78	// SXE3 8B97650
#define oCam_RotZDef			0x10FED98	// SXE3
#define oCam_RotY				0x10FEDA8	// SXE3
#define oCam_ClipX				0x1101EF8	// SXE3
#define oCam_ClipY				0x10FED60	// SXE3
#define oCam_ClipZ				0x1101F00	// SXE3
#define oCam_ClipXCS			0x1101ED0	// SXE3
#define oCam_ClipYCS			0x1101ED8	// SXE3
#define oCam_ClipX2				0x1101EF0	// SXE3 -- #?
#define oCam_ClipY2				0x1101EF8	// SXE3 -- #?
#define oCam_ClipZ2				0x1100154	// SXE3
#define oCam_ClipGL				0x10FCD74	// SXE3

#define ROTATION_SENS	5
#define ZOOM_MAX		60
#define ZOOM_MIN		15
#define ZOOM_INTER		1
#define CAMERA_DEBUG	1
#define PERCF(x, y)		((float)x / 100.0f * (float)y)
// ----------------------------------------------------------------------------------------------

typedef struct
{
	float	RotationY;
	float	RotationZ;
	float	PositionZ;
	double	ClipX;
	float	ClipY;
	double	ClipZ;
	double	ClipX2;
	float	ClipY2;
	float	ClipZ2;
	float	ClipGL;
} DefaultData, * lpDefData;
// ----------------------------------------------------------------------------------------------

class CameraV2
{
public:
				CameraV2();
				~CameraV2();
	// ----
	void		Load();
	void		Init();
	void		Switch();
	void		Run(MOUSEHOOKSTRUCTEX * Mouse, WPARAM wParam);
	void		Rotate();
	// ----
	static float __fastcall GetZoom(LPVOID This, LPVOID EDX);
	// ----
	bool		IsActive;
	float		ZoomPercent;
	float		Zoom;
	// ----
private:
	DefaultData Default;
	BYTE		RotateMode;
	bool		InMove;
	int			TempCursorX;
	int			TempCursorY;
	// ----
}; extern CameraV2 g_Camera;

#endif