#include "CharDeathConfirmationManager.h"

using namespace NewInterface;

static auto message = "Your character received a fatal blow and will be moved to the world map";

CharDeathConfirmationManager::CharDeathConfirmationManager(
	std::shared_ptr<ConfirmationModal> managedModal,
	std::shared_ptr<SharedNetwork> network
) {
	_managedModal = managedModal;
	_network = network;

	
	_managedModal->updateMessageTo(message);
	_managedModal->setHidden(true);
	_managedModal->onConfirmation([=](){
		_network->send(CTDS_USER_DIE_OK{});
	});
}
