#include "SceneObjects.h"
#include "EngineMath.h"

#include "Camera.h"

SceneObjects::SceneObjects(std::string pathToObjectsData, Graphics& gfx,
						   Rgph::BlurOutlineRenderGraph& rg)
	:
	sdr(pathToObjectsData),
	cameras(	sdr.GetCameraContainerPath(),	  gfx, rg),
	models(		sdr.GetModelContainerPath(),	  gfx, rg),
	pLight(     sdr.GetPointLightPath(),          gfx),
	plates(	    sdr.GetTriggerContainerPath(),	  gfx, rg)
{
	cameras.AddCamera(pLight.ShareCamera());
	rg.BindShadowCamera(*pLight.ShareCamera());
}

void SceneObjects::LinkTechniques(Rgph::RenderGraph& rg)
{
	pLight.LinkTechniques(rg);
	cameras.LinkTechniques();
	plates.LinkTechniques();
	models.LinkTechniques();
}

void SceneObjects::Submit(size_t channels)
{
	pLight.Submit(channels);
	cameras.Submit(channels);
	plates.Submit(channels);
	models.Submit(channels);
}