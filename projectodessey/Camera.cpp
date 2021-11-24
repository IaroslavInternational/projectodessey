#include "Camera.h"

#include "EngineMath.h"
#include "Graphics.h"

Camera::Camera(std::string		 name,
			   Graphics&		 gfx,
			   DirectX::XMFLOAT3 position,
			   DirectX::XMFLOAT3 orientation,
			   ProjectionData	 prd,
			   bool				 visibility,
			   bool				 tethered) noexcept
	:
	VisibleObject(name, position, orientation, visibility),
	homePosition(position),
	homeOrientation(orientation),
	proj(gfx, prd.width, prd.height, prd.nearZ, prd.farZ),
	indicator(gfx),
	tethered(tethered)
{
	if (tethered)
	{
		this->position = homePosition;

		indicator.SetPos(position);
		proj.SetPos(position);
	}

	this->orientation = homeOrientation;

	Reset(gfx);
}

void Camera::LinkTechniques(Rgph::RenderGraph& rg)
{
	indicator.LinkTechniques(rg);
	proj.LinkTechniques(rg);
}

void Camera::Submit(size_t channels)
{
	if (enableCameraIndicator)
	{
		indicator.Submit(channels);
	}
	if (enableFrustumIndicator)
	{
		proj.Submit(channels);
	}
}

void Camera::BindToGraphics(Graphics& gfx) const
{
	gfx.SetCamera(GetMatrix());
	gfx.SetProjection(proj.GetMatrix());
}

void Camera::Reset(Graphics& gfx) noexcept
{
	if (!tethered)
	{
		position = homePosition;

		indicator.SetPos(position);
		proj.SetPos(position);
	}

	orientation = homeOrientation;

	const DirectX::XMFLOAT3 angles = { orientation.x, orientation.y, 0.0f };

	indicator.SetRotation(angles);
	proj.SetRotation(angles);
	proj.Reset(gfx);
}

void Camera::Rotate(float dx, float dy) noexcept
{
	orientation.y = wrap_angle(orientation.y + dx * rotationSpeed);
	orientation.x = std::clamp(orientation.x + dy * rotationSpeed, 0.995f * -PI / 2.0f, 0.995f * PI / 2.0f);

	const DirectX::XMFLOAT3 angles = { orientation.x,orientation.y,0.0f };

	indicator.SetRotation(angles);
	proj.SetRotation(angles);
}

void Camera::Translate(DirectX::XMFLOAT3 translation) noexcept
{
	if (!tethered)
	{
		DirectX::XMStoreFloat3(&translation, DirectX::XMVector3Transform(
			DirectX::XMLoadFloat3(&translation),
			DirectX::XMMatrixRotationRollPitchYaw(orientation.x, orientation.y, 0.0f) *
			DirectX::XMMatrixScaling(travelSpeed, travelSpeed, travelSpeed)
		));

		position = {
			position.x + translation.x,
			position.y + translation.y,
			position.z + translation.z
		};

		indicator.SetPos(position);
		proj.SetPos(position);
	}
}

DirectX::XMMATRIX Camera::GetMatrix() const noexcept
{
	using namespace DirectX;

	const XMVECTOR forwardBaseVector = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	const auto lookVector = XMVector3Transform(forwardBaseVector,
		XMMatrixRotationRollPitchYaw(orientation.x, orientation.y, 0.0f)
	);
	const auto camPosition = XMLoadFloat3(&position);
	const auto camTarget = camPosition + lookVector;

	return XMMatrixLookAtLH(camPosition, camTarget, XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
}

ProjectionData Camera::GetProjectionData() const noexcept
{
	return proj.GetData();
}

DirectX::XMMATRIX Camera::GetProjection() const noexcept
{
	return proj.GetMatrix();
}

void Camera::SetPosition(DirectX::XMFLOAT3 position) noexcept
{
	this->position = position;

	indicator.SetPos(position);
	proj.SetPos(position);
}
