/*
 * Copyright (C) Elektrobit Automotive GmbH
 * All rights reserved
 */

package dreisoft.tresos.guidedconfig.demo6;

import dreisoft.tresos.guidedconfig.api.backend.AbstractBackend;
import dreisoft.tresos.guidedconfig.api.context.ECUConfigContext;
import dreisoft.tresos.guidedconfig.demo6.generated.operationstatus.MyGuidedConfigOperationStatus;
import dreisoft.tresos.lib2.api.log.APIOperationStatus;

/**
 * Backend class for demo6. The wizard is only available if the project has a configuration for the module
 * GCDemo6_Activator.
 *
 * <p>
 * <i> Copyright (C) Elektrobit Automotive GmbH - All rights reserved</i>
 * </p>
 *
 * @since 2009.a
 */
public class Demo6Backend extends AbstractBackend
{

    @Override
    public APIOperationStatus doIsAvailable()
    {
        // this code checks for a module with the ID "GCDemo6_Activator"
        ECUConfigContext context = (ECUConfigContext)getContexts().get(ECUConfigContext.ID);
        boolean containsModule = context.getModuleIds().contains("GCDemo6_Activator");
        if (containsModule)
        {
            // if the module is present and if it is enabled, the wizard can
            // show
            return null;
        }
        else
        {
            // if the module is not present, the message of the operation status
            // is displayed in a message box
            // and the wizard will not open
            return MyGuidedConfigOperationStatus.ERROR_OPEN_WIZARD();
        }
    }

}
