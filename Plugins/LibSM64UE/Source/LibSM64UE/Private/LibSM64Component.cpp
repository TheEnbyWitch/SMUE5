
#include "LibSM64Component.h"

#include "Core.h"
#include "FileHelpers.h"

#include "libsm64inc.h"
#include "libsm64/libsm64/src/decomp/include/surface_terrains.h"


// Copied from libsm64
#define NUM_USED_TEXTURES 11
#define ATLAS_WIDTH (NUM_USED_TEXTURES * 64)
#define ATLAS_HEIGHT 64

DEFINE_LOG_CATEGORY(LogLIBSM64);

void libsm64DebugPrint( const char * msg )
{
	UE_LOG(LogLIBSM64, Log, TEXT("%s"), UTF8_TO_TCHAR(msg));
}

void ULibSM64Component::SM64_GlobalInit(FString PathToRom)
{
	TArray<uint8> RomData;
	TArray<uint8> TextureData;
	TextureData.AddZeroed(SM64_TEXTURE_WIDTH * SM64_TEXTURE_HEIGHT * 4);

	FString Target = FPaths::ProjectContentDir() / PathToRom;
	
	FFileHelper::LoadFileToArray(RomData, *Target);
	sm64_global_init(RomData.GetData(),  TextureData.GetData(), libsm64DebugPrint);

	ROMTexture = UTexture2D::CreateTransient(ATLAS_WIDTH, ATLAS_HEIGHT, PF_R8G8B8A8, FName("T_MarioAtlas"));
	ROMTexture->MipGenSettings = TMGS_NoMipmaps;
	ROMTexture->AddressX = TA_Clamp;
	ROMTexture->AddressY = TA_Clamp;

	void* data = ROMTexture->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
	FMemory::Memcpy(data, TextureData.GetData(), TextureData.Num());
	ROMTexture->PlatformData->Mips[0].BulkData.Unlock();

	ROMTexture->UpdateResource();
}

void ULibSM64Component::SM64_GlobalTerminate()
{
	sm64_global_terminate();
}

void ULibSM64Component::SM64_BuildCollision()
{
	TArray<SM64Surface> surfArray;

	surfArray.Add(SM64Surface{SURFACE_DEEP_QUICKSAND, 0, 0, {
		{3200,-16,3200}, {3200, -16, -3200}, {-3200, -16, 3200}
		}
		});

	surfArray.Add(SM64Surface{SURFACE_DEFAULT, 0, 0, {
		{-3200, -16, 3200}, {3200, -16, -3200}, {-3200,-16,-3200}
		}
		});
	sm64_static_surfaces_load(surfArray.GetData(), surfArray.Num());
}
