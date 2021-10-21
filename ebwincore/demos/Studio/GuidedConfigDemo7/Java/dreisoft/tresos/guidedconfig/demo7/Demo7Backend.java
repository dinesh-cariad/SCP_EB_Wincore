/*
 * Copyright (C) Elektrobit Automotive GmbH
 * All rights reserved
 */

package dreisoft.tresos.guidedconfig.demo7;

import dreisoft.tresos.datamodel2.api.model.DCtxt;
import dreisoft.tresos.guidedconfig.api.backend.AbstractBackend;
import dreisoft.tresos.guidedconfig.api.context.ECUConfigContext;
import dreisoft.tresos.guidedconfig.api.memento.Memento;
import dreisoft.tresos.guidedconfig.api.pushservice.PushService;
import dreisoft.tresos.lib2.api.log.APIOperationStatus;

import java.util.List;

/**
 * The class is the controller of an autoconfigure wizard.
 *
 * <p>
 * An autoconfigure wizard is docked into the autoconfiguration functionality which can either be called via the
 * autoconfigure toolbar button or via commandline.
 * </p>
 *
 * <p>
 * Each wizard must implement an AbstractBackend that controls the lifecycle of a wizard.
 * </p>
 *
 * <p>
 * The backend has the following functionality:
 * </p>
 *
 * <ul>
 * <li>Receive all wizard lifecycle events including all GUI lifecycle events like page switches</li>
 * <li>Manages the private model aka. memento tree of the wizard</li>
 * <li>validates the private model of the wizard</li>
 * <li>controls the GUI of the wizard</li>
 * <li>provides the access to the project DataModel</li>
 * </ul>
 *
 * <p>
 * This backend is registered in the plugin.xml of this demo plug-in:
 * </p>
 *
 * <pre>
 *   &lt;extension
 *        point=&quot;dreisoft.tresos.guidedconfig.api.plugin.guidedconfigwizard&quot;&gt;
 *     ...
 *     &lt;guidedconfigwizard id=&quot;guidedDemo7&quot;&gt;
 *        &lt;backend class=&quot;test.tresos.plugin.guidedconfig.demo7.Demo7Backend&quot;&gt;
 *  ...
 * </pre>
 *
 * <p>
 * <i> Copyright (C) Elektrobit Automotive GmbH - All rights reserved</i>
 * </p>
 *
 * @since 2010.a
 */
public class Demo7Backend extends AbstractBackend
{

    //
    // defines

    /**
     * Path to the MODULE-DEF of the demo module
     */
    public static final String MODULE_SCHEMA_PATH = "/AUTOSAR/TOP-LEVEL-PACKAGES/TS_T00D0M0I0R0/ELEMENTS/GCDemo7";

    //
    // private members

    /**
     * The pushevent to reset the values on the tab "Stuff"
     */
    private ExampleAutoconfigureEvent m_pushEvent = null;

    @Override
    public List<APIOperationStatus> doRun()
    {
        // Get the root memento
        Memento rootMemento = getMemento();

        // Do not run operations when checkbox is not set
        if (!rootMemento.getBoolean(Demo7Page.MEMENTOID_CHECKBOX))
        {
            return null;
        }

        // Create a PushEvent that is sent to a PushOperation via the
        // PushService
        m_pushEvent = new ExampleAutoconfigureEvent();
        DCtxt targetConfigurationContext = getTargetConfigurationContext(MODULE_SCHEMA_PATH);
        if (targetConfigurationContext == null)
        {
            throw new IllegalArgumentException(
                "Cannot retrieve data context from path " + MODULE_SCHEMA_PATH);
        }
        m_pushEvent.setTargetContext(targetConfigurationContext);

        // Trigger the PushOperations
        PushService.getInstance().callSync(m_pushEvent, false);

        // if you use transactions instead of PushEvents:
        // 1. start Transaction like this:
        // context.getDCtxt().startTransaction( "MyTransaction" );
        // 2. do the actual work
        // 3. get all change-logs and add them to the result-widget
        // for( TransactionLog log : logs ) {
        // addChangedDCtxt( log.getChangedDCtxt() );
        // }
        // 4. finish Transaction:
        // context.getDCtxt().finishTransaction();

        // Return logged errors
        return m_pushEvent.getMessages();
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

    @Override
    public String doExportResults(List<APIOperationStatus> results)
    {
        StringBuffer buffer = new StringBuffer();
        for (DCtxt changedNode : m_pushEvent.getChangedNodes())
        {
            buffer.append("Changed value of " + changedNode.getPath() + " to " + changedNode.var.getValue() + "\n");
        }
        return buffer.toString();
    }

} // Demo7Backend

