/*
 * Copyright (C) Elektrobit Automotive GmbH
 * All rights reserved
 */
package dreisoft.tresos.guidedconfig.demo2;

import dreisoft.tresos.datamodel2.api.model.DCtxt;
import dreisoft.tresos.guidedconfig.api.backend.AbstractBackend;
import dreisoft.tresos.guidedconfig.api.context.ECUConfigContext;
import dreisoft.tresos.guidedconfig.api.gui.page.AbstractPage;
import dreisoft.tresos.guidedconfig.api.pushservice.AbstractPushEvent;
import dreisoft.tresos.guidedconfig.api.pushservice.PushService;

import java.util.List;

/**
 * This is a sample {@link AbstractBackend} implementation of the guidedconfig demo that is used for dialog based and
 * editor based guided configuration wizards.
 *
 * <p>
 * This demo implementation sends a {@link AbstractPushEvent} to retrieve configuration data from the project before the
 * GUI is shown.
 * </p>
 * <p>
 * <i> Copyright (C) Elektrobit Automotive GmbH - All rights reserved</i>
 * </p>
 *
 * @since 2009.a
 */
public class Demo2Backend extends AbstractBackend
{

    /**
     * Path to the MODULE-DEF of the demo module
     */
    public static final String MODULE_SCHEMA_PATH = "/AUTOSAR/TOP-LEVEL-PACKAGES/TS_T00D0M0I0R0/ELEMENTS/GCDemo2";

    @Override
    public void doShowPage(AbstractPage page)
    {
        // let a PushOperation fill part of the mementos of this wizard
        // the widgets filled from the datamodel are disabled, as we do not
        // send these data back to the datamodel while finishing

        ExamplePullEvent event = new ExamplePullEvent();
        event.setSourceMemento(getMemento());
        DCtxt targetConfigurationContext = getTargetConfigurationContext(MODULE_SCHEMA_PATH);
        if (targetConfigurationContext == null)
        {
            throw new IllegalArgumentException(
                "Cannot retrieve data context from path " + MODULE_SCHEMA_PATH);
        }
        event.setTargetContext(targetConfigurationContext);

        // Trigger PushOperations
        PushService.getInstance().callSync(event, false);
    }

    @Override
    public void doUpdateWidgetEnablement()
    {
        // disable all widgets in the datamodel group as the values may not be changed
        changeWidgetEnablement(Demo2Page.WIDGETID_CHECKBOX, false);
        changeWidgetEnablement(Demo2Page.WIDGETID_INT, false);
        changeWidgetEnablement(Demo2Page.WIDGETID_STRING, false);
        changeWidgetEnablement(Demo2Page.WIDGETID_FLOAT, false);
    }

    /**
     * Get the first configuration node of the project configuration to which this wizard belongs which matches the
     * given path in the schema (parameter definition).
     *
     * @param schemaPath The schema path
     * @return The referenced configuration node or null
     */
    public DCtxt getTargetConfigurationContext(String schemaPath)
    {
        ECUConfigContext ecuConfigContext = (ECUConfigContext)getContexts().get(ECUConfigContext.ID);
        List<DCtxt> dctxts = ecuConfigContext.getDCtxt().getFromSchema(schemaPath);
        return (dctxts != null) && (dctxts.size() > 0) ? dctxts.get(0) : null;
    }

} // Demo2Backend

