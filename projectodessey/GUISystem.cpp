#include "GUISystem.h"

#include "AdapterData.h"
#include "imgui\imgui.h"
#include "imgui\ImGuiFileDialog.h"

#include "EngineFunctions.hpp"
#include "Camera.h"

GUISystem::GUISystem(Scene* scene)
	:
	pLight(&scene->objects.pLight),
	models(&scene->objects.models),
	cameras(&scene->objects.cameras),
	triggers(&scene->objects.triggers),
	rb(&scene->robot)
{
	SetGuiColors();

	auto GPU_Data = AdapterReader::GetAdapterData();

	for (auto& d : GPU_Data)
	{
		if (d.desc.DedicatedVideoMemory != 0)
		{
			gpu_desc.emplace(std::wstring(d.desc.Description), round(static_cast<double>(d.desc.DedicatedVideoMemory) / 1073741824));
		}
	}
}

void GUISystem::Show()
{
	ShowMenu();

	if (IsShow)
	{
		ShowLeftPanel();
		ShowRightPanel();
		ShowLeftBottomPanel();
		ShowBottomPanel();
		ShowOptionalPanel();

		ShowRobotControl();
	}
}

void GUISystem::Hide()
{
	IsShow = false;
}

void GUISystem::SetPanelWidthAndPos(int corner, float width, float height, float x_offset, float y_offset)
{
	ImGuiIO& io = ImGui::GetIO();

	float MenuHeight = ImGui::GetMenuHeight();
	ImVec2 DispSize = io.DisplaySize;

	float PanelW = round(DispSize.x * width);
	float PanelH = DispSize.y * height;

	ImVec2 PanelSize = ImVec2(
		PanelW,
		PanelH
	);

	ImVec2 PanelPos = ImVec2(
		(corner & 1) ? DispSize.x + round(DispSize.x * x_offset) : round(DispSize.x * x_offset),
		(corner & 2) ? DispSize.y + MenuHeight + DispSize.y * y_offset : MenuHeight + DispSize.y * y_offset
	);

	ImVec2 PanelPivot = ImVec2(
		(corner & 1) ? 1.0f : 0.0f,
		(corner & 2) ? 1.0f : 0.0f
	);

	ImGui::SetNextWindowPos(PanelPos, ImGuiCond_Always, PanelPivot);
	ImGui::SetNextWindowSize(PanelSize);
}

void GUISystem::ShowMenu()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Файл"))
		{
			if (ImGui::MenuItem("Выход"))
			{
				exit(0);
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Окна"))
		{
			if (ImGui::MenuItem("Освещение"))
			{
				ShowPLightSettings ? ShowPLightSettings = false : ShowPLightSettings = true;
			}

			if (ImGui::BeginMenu("Объекты"))
			{
				if (ImGui::MenuItem("Триггеры"))
				{
					if (ShowTriggersSettings)
					{
						DisableSides();

						ShowTriggersSettings = false;
					}
					else
					{
						DisableSides();

						ShowTriggersSettings = true;
					}
				}

				if (ImGui::MenuItem("Модели"))
				{
					if (ShowModelsSettings)
					{
						DisableSides();

						ShowModelsSettings = false;
					}
					else
					{
						DisableSides();

						ShowModelsSettings = true;
					}
				}

				if (ImGui::MenuItem("Камеры"))
				{
					if (ShowCamerasSettings)
					{
						DisableSides();

						ShowCamerasSettings = false;
					}
					else
					{
						DisableSides();

						ShowCamerasSettings = true;
					}
				}

				ImGui::EndMenu();
			}

			if (ImGui::MenuItem("FPS & GPU"))
			{
				ShowHardwareInfo ? ShowHardwareInfo = false : ShowHardwareInfo = true;
			}

			if (ImGui::MenuItem("Лог"))
			{
				ShowLogs ? ShowLogs = false : ShowLogs = true;
			}

			if (ImGui::MenuItem("Отключить панели"))
			{
				DisableSides();
			}

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
}

void GUISystem::ShowLeftPanel()
{
	SetPanelWidthAndPos(0, 0.2f, 0.75f);

	if (ShowModelsSettings)
	{
		ShowModelsList();
	}
	else if (ShowTriggersSettings)
	{
		ShowTriggersList();
	}
	else if (ShowCamerasSettings)
	{
		ShowCamerasList();
	}

	ImGui::GetStyle().DisplayWindowPadding   = { 0.0f, 0.0f };
	ImGui::GetStyle().DisplaySafeAreaPadding = { 0.0f, 0.0f };
}

void GUISystem::ShowRightPanel()
{
	ImGuiIO& io = ImGui::GetIO();

	int corner = 1;

	float MenuHeight = ImGui::GetMenuHeight();
	float RightPanelW = io.DisplaySize.x * 0.2f;

	ImVec2 RightPanelPivot = ImVec2(
		(corner & 1) ? 1.0f : 0.0f,
		(corner & 2) ? 1.0f : 0.0f
	);

	SetPanelWidthAndPos(corner, 0.2f, 1.0f);

	if (ShowModelsSettings)
	{
		ShowModelsControl();
	}
	else if (ShowTriggersSettings)
	{
		ShowTriggersControl();
	}
	else if (ShowCamerasSettings)
	{
		ShowCamerasControl();
	}
}

void GUISystem::ShowLeftBottomPanel()
{
	SetPanelWidthAndPos(2, 0.2f, 0.25f);

	if (ShowHardwareInfo)
	{
		ShowFPSAndGPU();
	}
}

void GUISystem::ShowBottomPanel()
{
	/* Нижняя стороны */

	SetPanelWidthAndPos(3, 0.6f, 0.25f, -0.2f);

	/* Содержимое */

	if (ShowLogs)
	{
		ShowLog();
	}

	/**************/

	/* Конец нижней стороны */
}

void GUISystem::ShowFPSAndGPU()
{
	if (ImGui::Begin("Представление", NULL,
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize |
		ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoBringToFrontOnFocus))
	{
		ImGui::Text("%.3f мс/кадр (%.2f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

		std::ostringstream s;
		s << sizeof(*this);

		ImGui::Text("Вес GUI: %s байт", s.str().c_str());

		ImGui::Separator();

		ImGui::Text("Графическое оборудование:");
		for (auto& d : gpu_desc)
		{
			char name_gpu[256];
			sprintf_s(name_gpu, "%ws", reinterpret_cast<const char*>(d.first.c_str()));

			if (ImGui::TreeNode(name_gpu))
			{
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.52f, 0.85f, 0.059, 1.0f));
				ImGui::Text("Память: ~%.1f ГБ", d.second);
				ImGui::PopStyleColor();
				ImGui::TreePop();
			}
		}
	}

	ImGui::End();
}

void GUISystem::ShowOptionalPanel()
{
	if (ShowPLightSettings)
	{
		ShowPLightControl();
	}
}

void GUISystem::ShowLog()
{
	ImGui::Begin("Лог", NULL, ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize |
		ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoBringToFrontOnFocus);

	applog.Draw("Лог", NULL);
	ImGui::End();
}

void GUISystem::DisableSides()
{
	ShowModelsSettings   = false;
	ShowTriggersSettings = false;
	ShowPLightSettings   = false;
	ShowCamerasSettings	 = false;
}

void GUISystem::SetGuiColors()
{
	/*Стиль интерфейса*/

	ImGui::GetStyle().FrameRounding    = 4.0f;								// Закругление компонентов
	ImGui::GetStyle().WindowBorderSize = 0.0f;								// Размер границы
	ImGui::GetStyle().WindowRounding   = 10.0f;								// Закругление окон

	// Цвета
	ImVec4* colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.15f, 0.36f, 0.39f, 1.00f);		// Главное меню
	colors[ImGuiCol_WindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.84f);			// Фон окна
	colors[ImGuiCol_TitleBg] = ImVec4(0.24f, 0.00f, 0.20f, 0.73f);			// Меню окна
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.15f, 0.00f, 0.07f, 0.73f);	// Наведение на меню окна
	colors[ImGuiCol_FrameBg] = ImVec4(0.00f, 0.50f, 0.38f, 0.54f);			// Компонента
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.00f, 0.18f, 0.15f, 0.40f);	// Наведение на компоненту
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.06f, 0.48f, 0.45f, 0.67f);	// Активные компонента
	colors[ImGuiCol_CheckMark] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);		// Галочка
	colors[ImGuiCol_SliderGrab] = ImVec4(0.37f, 0.70f, 0.00f, 1.00f);		// Ползунок слайдера
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.62f, 0.82f, 0.19f, 1.00f);	// Актвиный ползунок слайдера
	colors[ImGuiCol_Button] = ImVec4(0.56f, 0.05f, 0.05f, 0.59f);			// Кнопка
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.26f, 0.01f, 0.17f, 1.00f);	// Наведение на кнопку
	colors[ImGuiCol_ButtonActive] = ImVec4(0.03f, 0.55f, 0.48f, 1.00f);		// Активная кнопка
	colors[ImGuiCol_Separator] = ImVec4(0.66f, 0.60f, 0.00f, 0.50f);		// Разделитель
	colors[ImGuiCol_Tab] = ImVec4(0.00f, 0.08f, 0.27f, 0.86f);				// Раздел
	colors[ImGuiCol_TabHovered] = ImVec4(0.01f, 0.43f, 0.63f, 0.80f);		// Наведение на раздел
	colors[ImGuiCol_TabActive] = ImVec4(0.66f, 0.60f, 0.00f, 0.50f);		// Активный раздел
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.19f, 0.67f, 0.65f, 1.00f);	// Ползунок
	colors[ImGuiCol_TableHeaderBg] = ImVec4(0.31f, 0.04f, 0.04f, 0.81f);	// Блок заголовка таблицы
	colors[ImGuiCol_Header] = ImVec4(0.50f, 0.09f, 0.70f, 0.31f);			// Заголовок
	colors[ImGuiCol_HeaderActive] = ImVec4(0.00f, 0.57f, 0.49f, 1.00f);		// Активный заголовок
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.40f, 0.22f, 0.59f, 0.80f);	// Наведение на заголовк

	/******************/
}

void GUISystem::ShowModelsList()
{
	if (ImGui::Begin("Объекты", NULL,
		ImGuiWindowFlags_NoMove   | ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus))
	{
		for (auto& model : models->models)
		{
			char label[128];
			sprintf_s(label, model->name.c_str(), mSelected);

			ImGui::Bullet();
			if (ImGui::Selectable(label, mSelected == model->name.c_str()))
			{
				mSelected = model->name;
			}
		}

		ImGui::NewLine();

		if (ImGui::Button("Добавить"))
		{
			ImGuiFileDialog::Instance()->OpenDialog("ModelOD", "Выбирете файл", ".obj,.mtl,.gltf,.glb,.fbx", "");
		}

		//OpenDialog();
	}

	ImGui::End();
}

void GUISystem::ShowTriggersList()
{
	if (ImGui::Begin("Триггеры", NULL,
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoBringToFrontOnFocus))
	{
		for (auto& t : triggers->triggers)
		{
			char label[128];
			sprintf_s(label, t->name.c_str(), tSelected.c_str());

			ImGui::Bullet();
			if (ImGui::Selectable(label, tSelected == t->name.c_str()))
			{
				tSelected = t->name;
			}
		}

		if (ImGui::Button("Добавить"))
		{
		}
	}

	ImGui::End();
}

void GUISystem::ShowCamerasList()
{
	if (ImGui::Begin("Камеры", NULL,
		ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus))
	{
		for (size_t i = 0; i < cameras->cameras.size(); i++)
		{
			char label[128];
			sprintf_s(label, cameras->GetCameraNameByIndex(i).c_str(), cSelected);

			ImGui::Bullet();
			if (ImGui::Selectable(label, cSelected == cameras->GetCameraNameByIndex(i).c_str()))
			{
				cSelected = cameras->GetCameraNameByIndex(i).c_str();
				cameras->active = i;
				cameras->controlled = i;
			}
		}

		ImGui::NewLine();

		if (ImGui::Button("Добавить"))
		{
			cameras->AddRuntimeCamera(std::make_shared<Camera>(std::string("Camera ") + std::to_string(cameras->cameras.size() + 1), cameras->gfx));
		}
	}

	ImGui::End();
}

void GUISystem::ShowModelsControl()
{
	if (ImGui::Begin("Настройки", NULL,
		ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus))
	{
		for (int k = 0; k < models->models.size(); k++)
		{
			if (models->models[k]->name == mSelected)
			{
				if (ImGui::BeginChild(""))
				{
					bool rotDirty = false;
					bool posDirty = false;
					bool scaleDirty = false;
					bool animDirty = false;

					const auto dcheck = [](bool d, bool& carry) { carry = carry || d; };

					ImGui::Text("Позиция");
					dcheck(ImGui::SliderFloat("X", &models->models[k]->position.x, -80.0f, 80.0f, "%.1f"), posDirty);
					dcheck(ImGui::SliderFloat("Y", &models->models[k]->position.y, -80.0f, 80.0f, "%.1f"), posDirty);
					dcheck(ImGui::SliderFloat("Z", &models->models[k]->position.z, -80.0f, 80.0f, "%.1f"), posDirty);

					ImGui::Text("Ориентация");
					dcheck(ImGui::SliderAngle("Крен",     &models->models[k]->orientation.x, 0.995f * -90.0f, 0.995f * 90.0f), rotDirty);
					dcheck(ImGui::SliderAngle("Тангаш",   &models->models[k]->orientation.y, 0.995f * -90.0f, 0.995f * 90.0f), rotDirty);
					dcheck(ImGui::SliderAngle("Расканье", &models->models[k]->orientation.z, -180.0f, 180.0f), rotDirty);

					ImGui::Checkbox("Скрыть", &models->models[k]->visibility);

					models->models[k]->SetRootTransform
					(
						DirectX::XMMatrixRotationX(models->models[k]->orientation.x) *
						DirectX::XMMatrixRotationY(models->models[k]->orientation.y) *
						DirectX::XMMatrixRotationZ(models->models[k]->orientation.z) *
						DirectX::XMMatrixTranslation(
							models->models[k]->position.x,
							models->models[k]->position.y,
							models->models[k]->position.z
						)
					);

					ImGui::EndChild();
				}

				if (IsSave)
				{
					auto position    = models->models[k]->position;
					auto orientation = models->models[k]->orientation;
					auto name		 = models->models[k]->name;

					/*EngineFunctions::SetNewValue<float>(name, "pos-x", position.x, path, applog);
					EngineFunctions::SetNewValue<float>(name, "pos-y", position.y, path, applog);
					EngineFunctions::SetNewValue<float>(name, "pos-z", position.z, path, applog);

					EngineFunctions::SetNewValue<float>(name, "roll", orientation.x, path, applog);
					EngineFunctions::SetNewValue<float>(name, "pitch", orientation.y, path, applog);
					EngineFunctions::SetNewValue<float>(name, "yaw", orientation.z, path, applog);*/

					IsSave = false;
				}

				if (IsDelete)
				{
					models->DeleteModel(mSelected);

					IsDelete = false;
				}

				break;
			}
		}

		if (ImGui::Button("Удалить", ImVec2(100, 20)))
		{
			IsDelete = true;
		}

		ImGui::SameLine();

		if (ImGui::Button("Сохранить", ImVec2(100, 20)))
		{
			IsSave = true;
		}
	}

	ImGui::End();
}

void GUISystem::ShowTriggersControl()
{
	if (ImGui::Begin("Настройки", NULL,
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoBringToFrontOnFocus))
	{
		for (auto& t : triggers->triggers)
		{
			if (t->name == tSelected)
			{
				bool rotDirty = false;
				bool posDirty = false;
				bool szDirty = false;
				bool dDirty = false;

				const auto dcheck = [](bool d, bool& carry) { carry = carry || d; };

				ImGui::Text("Позиция");
				dcheck(ImGui::SliderFloat("X", &t->position.x, -80.0f, 80.0f, "%.2f"), posDirty);
				dcheck(ImGui::SliderFloat("Y", &t->position.y, -80.0f, 80.0f, "%.2f"), posDirty);
				dcheck(ImGui::SliderFloat("Z", &t->position.z, -80.0f, 80.0f, "%.2f"), posDirty);

				ImGui::Text("Ориентация");
				dcheck(ImGui::SliderAngle("Roll", &t->orientation.x,  -180.0f, 180.0f, "%.2f"), rotDirty);
				dcheck(ImGui::SliderAngle("Pitch", &t->orientation.y, -180.0f, 180.0f, "%.2f"), rotDirty);
				dcheck(ImGui::SliderAngle("Yaw", &t->orientation.z,   -180.0f, 180.0f, "%.2f"), rotDirty);

				ImGui::Text("Глубина:");
				dcheck(ImGui::SliderFloat("", &t->deep, 1.0f, 5.0f, "%.2f"), dDirty);

				t->platform.SetPosition(t->position);
				t->platform.SetRotation(t->orientation);
				
				ImGui::NewLine();

				ImGui::Separator();
				ImGui::Text("Триггер указывает на: %s", "GOAL HERE");

				break;
			}
		}
	}

	ImGui::End();
}

void GUISystem::ShowCamerasControl()
{
	if (ImGui::Begin("Опции", NULL,
		ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus))
	{
		for (int k = 0; k < cameras->cameras.size(); k++)
		{
			if (cameras->GetCameraNameByIndex(k) == cSelected)
			{
				bool rotDirty = false;
				bool posDirty = false;

				const auto dcheck = [](bool d, bool& carry) { carry = carry || d; };

				if (!cameras->GetControlledCamera().tethered)
				{
					ImGui::Text("Позиция");
					dcheck(ImGui::SliderFloat("X", &cameras->GetControlledCamera().position.x, -80.0f, 80.0f, "%.1f"), posDirty);
					dcheck(ImGui::SliderFloat("Y", &cameras->GetControlledCamera().position.y, -80.0f, 80.0f, "%.1f"), posDirty);
					dcheck(ImGui::SliderFloat("Z", &cameras->GetControlledCamera().position.z, -80.0f, 80.0f, "%.1f"), posDirty);
				}

				ImGui::Text("Ориентация");
				dcheck(ImGui::SliderAngle("Тангаж",   &cameras->GetControlledCamera().orientation.x, 0.995f * -90.0f, 0.995f * 90.0f), rotDirty);
				dcheck(ImGui::SliderAngle("Расканье", &cameras->GetControlledCamera().orientation.y, -180.0f, 180.0f), rotDirty);

				cameras->GetControlledCamera().proj.RenderWidgets(cameras->gfx); // !!!

				ImGui::Checkbox("Индикатор",   &cameras->GetControlledCamera().enableCameraIndicator);
				ImGui::Checkbox("Отображение", &cameras->GetControlledCamera().enableFrustumIndicator);

				if (ImGui::Button("Сбросить"))
				{
					cameras->GetControlledCamera().Reset(cameras->gfx); // !!!
				}

				if (rotDirty)
				{
					const DirectX::XMFLOAT3 angles = { cameras->GetControlledCamera().orientation.x, cameras->GetControlledCamera().orientation.y, 0.0f };
					cameras->GetControlledCamera().indicator.SetRotation(angles);
					cameras->GetControlledCamera().proj.SetRotation(angles);
				}

				if (posDirty)
				{
					cameras->GetControlledCamera().indicator.SetPos(cameras->GetControlledCamera().position);
					cameras->GetControlledCamera().proj.SetPos(cameras->GetControlledCamera().position);
				}

				if (IsSave)
				{				
					auto position = cameras->GetActiveCamera().GetPosition();
					auto orientation = cameras->GetActiveCamera().GetOrientation();
					auto proj = cameras->GetActiveCamera().GetProjectionData();
					auto name = cameras->GetActiveCamera().name;

					/*EngineFunctions::SetNewValue<float>(name, "pos-x", position.x, path, applog);
					EngineFunctions::SetNewValue<float>(name, "pos-y", position.y, path, applog);
					EngineFunctions::SetNewValue<float>(name, "pos-z", position.z, path, applog);

					EngineFunctions::SetNewValue<float>(name, "pitch", orientation.x, path, applog);
					EngineFunctions::SetNewValue<float>(name, "yaw",   orientation.y, path, applog);

					EngineFunctions::SetNewValue<float>(name, "proj-width",  proj.width, path, applog);
					EngineFunctions::SetNewValue<float>(name, "proj-height", proj.height, path, applog);
					EngineFunctions::SetNewValue<float>(name, "proj-farZ",   proj.farZ, path, applog);
					EngineFunctions::SetNewValue<float>(name, "proj-nearZ",  proj.nearZ, path, applog);*/

					IsSave = false;
				}

				if (IsDelete)
				{
					cameras->DeleteCamera(cameras->cameras[k]->name);
					IsDelete = false;
				}

				break;
			}
		}

		if (ImGui::Button("Удалить", ImVec2(100, 20)))
		{
			IsDelete = true;
		}

		ImGui::SameLine();

		if (ImGui::Button("Сохранить", ImVec2(100, 20)))
		{
			IsSave = true;
		}
	}

	ImGui::End();
}

void GUISystem::ShowPLightControl()
{
	ImGuiIO& io = ImGui::GetIO();
	ImVec2 DispSize = io.DisplaySize;

	ImVec2 PanelSize = ImVec2(
		round(DispSize.x * 0.25f),
		DispSize.y * 0.5f
	);

	ImGui::SetNextWindowSize(PanelSize);
	if (ImGui::Begin("Свет", &ShowPLightSettings, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse))
	{
		bool dirtyPos = false;
		bool dirtyRender = false;

		const auto dP = [&dirtyPos](bool dirty) {dirtyPos = dirtyPos || dirty; };
		const auto dR = [&dirtyRender](bool dirty) {dirtyRender = dirtyRender || dirty; };
		
		ImGui::Text("Позиция");
		dP(ImGui::SliderFloat("X", &pLight->cbData.pos.x, -80.0f, 80.0f, "%.4f"));
		dP(ImGui::SliderFloat("Y", &pLight->cbData.pos.y, -80.0f, 80.0f, "%.4f"));
		dP(ImGui::SliderFloat("Z", &pLight->cbData.pos.z, -80.0f, 80.0f, "%.4f"));

		if (dirtyPos)
		{
			pLight->pCamera->SetPosition(pLight->cbData.pos);
		}

		ImGui::Text("Интенсивность и цвет");
		ImGui::SliderFloat("Интенсивность", &pLight->cbData.diffuseIntensity, 0.01f, 2.0f, "%.2f", 2);
		ImGui::ColorEdit3("Цвет",			&pLight->cbData.diffuseColor.x);
		ImGui::ColorEdit3("Цвет среды",	    &pLight->cbData.ambient.x);

		ImGui::Text("Тень");
		ImGui::SliderFloat("Constant",  &pLight->cbData.attConst, 0.05f, 10.0f, "%.2f", 4);
		ImGui::SliderFloat("Linear",    &pLight->cbData.attLin, 0.0001f, 4.0f, "%.4f", 8);
		ImGui::SliderFloat("Quadratic", &pLight->cbData.attQuad, 0.00000001f, 0.2f, "%.8f", 10);

		ImGui::Checkbox("Скрыть", &pLight->IsRendered);

		if (ImGui::Button("Сбросить"))
		{
			pLight->Reset();
		}
	}

	ImGui::End();
}

void GUISystem::ShowRobotControl()
{
	ImGuiIO& io = ImGui::GetIO();
	ImVec2 DispSize = io.DisplaySize;

	ImVec2 PanelSize = ImVec2(
		round(DispSize.x * 0.25f),
		DispSize.y * 0.5f
	);

	ImGui::SetNextWindowSize(PanelSize);
	if (ImGui::Begin("ТНПА", &ShowRobotSettings, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse))
	{
		bool rotDirty = false;
		bool posDirty = false;
		bool scaleDirty = false;

		const auto dcheck = [](bool d, bool& carry) { carry = carry || d; };
		
		ImGui::Text("Позиция");
		dcheck(ImGui::SliderFloat("X", &rb->model->position.x, -80.0f, 80.0f, "%.4f"), posDirty);
		dcheck(ImGui::SliderFloat("Y", &rb->model->position.y, -80.0f, 80.0f, "%.4f"), posDirty);
		dcheck(ImGui::SliderFloat("Z", &rb->model->position.z, -80.0f, 80.0f, "%.4f"), posDirty);

		ImGui::Text("Ориентация");
		dcheck(ImGui::SliderAngle("Крен", &rb->model->orientation.x, 0.995f * -90.0f, 0.995f * 90.0f), rotDirty);
		dcheck(ImGui::SliderAngle("Тангаш", &rb->model->orientation.y, 0.995f * -90.0f, 0.995f * 90.0f), rotDirty);
		dcheck(ImGui::SliderAngle("Расканье", &rb->model->orientation.z, -180.0f, 180.0f), rotDirty);

		ImGui::Checkbox("Скрыть", &rb->model->visibility);

		rb->model->SetRootTransform
		(
			DirectX::XMMatrixRotationX(rb->model->orientation.x) *
			DirectX::XMMatrixRotationY(rb->model->orientation.y) *
			DirectX::XMMatrixRotationZ(rb->model->orientation.z) *
			DirectX::XMMatrixTranslation(
				rb->model->position.x,
				rb->model->position.y,
				rb->model->position.z
			)
		);
	}

	ImGui::End();
}
