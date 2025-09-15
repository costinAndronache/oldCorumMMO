#pragma once
#include "../ConfirmationModal.h"
#include "../../NewHelpers/SharedNetwork.h"

namespace NewInterface {
	class CharDeathConfirmationManager {
	public:
		CharDeathConfirmationManager(
			std::shared_ptr<ConfirmationModal> managedModal,
			std::shared_ptr<SharedNetwork> network
		);

	private:
		std::shared_ptr<ConfirmationModal> _managedModal;
		std::shared_ptr<SharedNetwork> _network;
	};
}


