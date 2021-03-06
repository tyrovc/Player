/////////////////////////////////////////////////////////////////////////////
// This file is part of EasyRPG Player.
// 
// EasyRPG Player is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// EasyRPG Player is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with EasyRPG Player. If not, see <http://www.gnu.org/licenses/>.
/////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include "game_temp.h"
#include "game_system.h"
#include "game_party.h"
#include "input.h"
#include "scene_shop.h"
#include "output.h"
#include "time.hpp"

////////////////////////////////////////////////////////////
Scene_Shop::Scene_Shop() :
	help_window(NULL),
	buy_window(NULL),
	party_window(NULL),
	status_window(NULL),
	gold_window(NULL),
	sell_window(NULL),
	number_window(NULL),
	empty_window(NULL),
	empty_window2(NULL),
	shop_window(NULL) {
	Scene::type = Scene::Shop;
}

////////////////////////////////////////////////////////////
void Scene_Shop::Start() {
	shop_window = new Window_Shop(0, 160, 320, 80);
	help_window = new Window_Help(0, 0, 320, 32);
	gold_window = new Window_Gold(184, 128, 136, 32);
	empty_window = new Window_Base(0, 32, 320, 128);
	empty_window2 = new Window_Base(0, 32, 184, 128);
	buy_window = new Window_ShopBuy(0, 32, 184, 128);
	party_window = new Window_ShopParty(184, 32, 136, 48);
	sell_window = new Window_ShopSell(0, 32, 320, 128);
	status_window = new Window_ShopStatus(184, 80, 136, 48);
	number_window = new Window_ShopNumber(0, 32, 184, 128);

	buy_window->SetActive(false);
	buy_window->SetVisible(false);
	buy_window->SetHelpWindow(help_window);

	sell_window->SetActive(false);
	sell_window->SetVisible(false);
	sell_window->SetHelpWindow(help_window);
	
	number_window->SetActive(false);
	number_window->SetVisible(false);

	status_window->SetVisible(false);
	sell_window->Refresh();
	sell_window->SetIndex(0);
	sell_window->SetActive(true);

	Game_Temp::shop_transaction = false;
	timer = 0;

	if (Game_Temp::shop_buys && Game_Temp::shop_sells) {
		SetMode(BuySellLeave);
	} else if (Game_Temp::shop_buys) {
		shop_window->SetChoice(Buy);
		SetMode(Buy);
	} else {
		shop_window->SetChoice(Sell);
		SetMode(Sell);
	}
}

////////////////////////////////////////////////////////////
void Scene_Shop::Terminate() {
	delete shop_window;
	delete help_window;
	delete gold_window;
	delete empty_window;
	delete empty_window2;
	delete buy_window;
	delete party_window;	
	delete sell_window;
	delete status_window;
	delete number_window;
}

////////////////////////////////////////////////////////////
static void Enable(Window* window, bool state) {
	window->SetVisible(state);
	window->SetActive(state);
}

void Scene_Shop::SetMode(int nmode) {
	mode = nmode;
	help_window->SetText("");

	// Central panel
	switch (mode) {
		case BuySellLeave:
		case BuySellLeave2:
			empty_window->SetVisible(true);
			Enable(sell_window, false);
			break;
		case Sell:
			empty_window->SetVisible(false);
			Enable(sell_window, true);
			break;
		case Buy:
		case BuyHowMany:
		case SellHowMany:
		case Bought:
		case Sold:
			empty_window->SetVisible(false);
			Enable(sell_window, false);
			break;
	}

	// Right-hand panels
	switch (mode) {
		case BuySellLeave:
		case BuySellLeave2:
		case Sell:
			party_window->SetVisible(false);
			status_window->SetVisible(false);
			gold_window->SetVisible(false);
			break;
		case Buy:
		case BuyHowMany:
		case SellHowMany:
		case Bought:
		case Sold:
			party_window->SetVisible(true);
			status_window->SetVisible(true);
			gold_window->SetVisible(true);
			break;
	}

	// Left-hand panels
	switch (mode) {
		case BuySellLeave:
		case BuySellLeave2:
		case Sell:
			Enable(buy_window, false);
			Enable(number_window, false);
			empty_window2->SetVisible(false);
			break;
		case Buy:
			buy_window->Refresh();
			Enable(buy_window, true);
			Enable(number_window, false);
			empty_window2->SetVisible(false);
			break;
		case BuyHowMany:
		case SellHowMany:
			number_window->Refresh();
			Enable(buy_window, false);
			Enable(number_window, true);
			break;
		case Bought:
		case Sold:
			Enable(buy_window, false);
			Enable(number_window, false);
			empty_window2->SetVisible(true);

			timer = DEFAULT_FPS;
			break;
	}

	shop_window->SetMode(mode);
}

////////////////////////////////////////////////////////////
void Scene_Shop::Update() {
	buy_window->Update();
	sell_window->Update();
	shop_window->Update();
	number_window->Update();
	party_window->Update();

	switch (mode) {
		case BuySellLeave:
		case BuySellLeave2:
			UpdateCommandSelection();
			break;
		case Buy:
			UpdateBuySelection();
			break;
		case Sell:
			UpdateSellSelection();
			break;
		case BuyHowMany:
		case SellHowMany:
			UpdateNumberInput();
			break;
		case Bought:
			timer--;
			if (timer == 0)
				SetMode(Buy);
			break;
		case Sold:
			timer--;
			if (timer == 0)
				SetMode(Sell);
			break;
		default:
			break;
	}
}

////////////////////////////////////////////////////////////
void Scene_Shop::UpdateCommandSelection() {
	if (Input::IsTriggered(Input::CANCEL)) {
		Game_System::SePlay(Data::system.cancel_se);
		Scene::Pop();
	} else if (Input::IsTriggered(Input::DECISION)) {
		switch (shop_window->GetChoice()) {
			case Buy:
			case Sell:
				SetMode(shop_window->GetChoice());
				break;
			case Leave:
				Scene::Pop();
				break;
		}
	}
}

void Scene_Shop::UpdateBuySelection() {
	status_window->SetItemId(buy_window->GetItemId());
	party_window->SetItemId(buy_window->GetItemId());

	if (Input::IsTriggered(Input::CANCEL)) {
		Game_System::SePlay(Data::system.cancel_se);
		if (Game_Temp::shop_sells) {
			SetMode(BuySellLeave2);
		} else {
			Scene::Pop();
		}
	} else if (Input::IsTriggered(Input::DECISION)) {
		int item_id = buy_window->GetItemId();

		//checks the money and number of items possessed before buy
		if (buy_window->CheckEnable(item_id)) {
			Game_System::SePlay(Data::system.decision_se);

			RPG::Item& item = Data::items[item_id - 1];

			int max;
			if (item.price == 0) {
				max = 99;
			} else {
				max = Game_Party::GetGold() / item.price;
			}
			number_window->SetData(item_id, max, item.price);

			SetMode(BuyHowMany);
		}
		else {
			Game_System::SePlay(Data::system.buzzer_se);
		}
	}
}

void Scene_Shop::UpdateSellSelection() {
	if (Input::IsTriggered(Input::CANCEL)) {
		Game_System::SePlay(Data::system.cancel_se);
		if (Game_Temp::shop_buys) {
			SetMode(BuySellLeave2);
		} else {
			Scene::Pop();
		}
	} else if (Input::IsTriggered(Input::DECISION)) {
		int item_id = sell_window->GetItemId();
		status_window->SetItemId(item_id);
		party_window->SetItemId(item_id);

		if (item_id > 0 && Data::items[item_id - 1].price > 0) {
			RPG::Item& item = Data::items[item_id - 1];
			Game_System::SePlay(Data::system.decision_se);
			number_window->SetData(item_id, Game_Party::ItemNumber(item_id), item.price);
			SetMode(SellHowMany);
		}
		else {
			Game_System::SePlay(Data::system.buzzer_se);
		}
	}
}

void Scene_Shop::UpdateNumberInput() {
	if (Input::IsTriggered(Input::CANCEL)) {
		Game_System::SePlay(Data::system.cancel_se);
		switch (shop_window->GetChoice()) {
		case Buy:
			SetMode(Buy); break;
		case Sell:
			SetMode(Sell); break;
		}
	} else if (Input::IsTriggered(Input::DECISION)) {
		int item_id;
		switch (shop_window->GetChoice()) {
		case Buy:
			item_id = buy_window->GetItemId();
			Game_Party::LoseGold(number_window->GetTotal());
			Game_Party::GainItem(item_id, number_window->GetNumber());
			gold_window->Refresh();
			buy_window->Refresh();
			status_window->Refresh();
			SetMode(Bought); break;
		case Sell:
			item_id = sell_window->GetItemId();
			Game_Party::GainGold(number_window->GetTotal());
			Game_Party::LoseItem(item_id, number_window->GetNumber());
			gold_window->Refresh();
			sell_window->Refresh();
			status_window->Refresh();
			SetMode(Sold); break;
		}
		Game_System::SePlay(Data::system.decision_se);

		Game_Temp::shop_transaction = true;
	}
}
