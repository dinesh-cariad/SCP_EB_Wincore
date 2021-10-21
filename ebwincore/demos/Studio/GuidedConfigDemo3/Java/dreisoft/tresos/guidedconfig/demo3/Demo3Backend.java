/*
 * Copyright (C) Elektrobit Automotive GmbH
 * All rights reserved
 */

package dreisoft.tresos.guidedconfig.demo3;

import dreisoft.tresos.datamodel2.api.model.DCtxt;
import dreisoft.tresos.datamodel2.api.model.TransactionLog;
import dreisoft.tresos.guidedconfig.api.backend.AbstractBackend;
import dreisoft.tresos.guidedconfig.api.context.ECUConfigContext;
import dreisoft.tresos.guidedconfig.api.memento.Memento;
import dreisoft.tresos.guidedconfig.api.pushservice.PushService;
import dreisoft.tresos.lib2.api.log.APIOperationStatus;

import java.util.ArrayList;
import java.util.List;
import java.util.Vector;

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
 *     &lt;guidedconfigwizard id=&quot;guidedDemo3&quot;&gt;
 *        &lt;backend class=&quot;test.tresos.plugin.guidedconfig.demo3.Demo3Backend&quot;&gt;
 *  ...
 * </pre>
 *
 * <p>
 * <i> Copyright (C) Elektrobit Automotive GmbH - All rights reserved</i>
 * </p>
 *
 * @since 2009.a
 */
public class Demo3Backend extends AbstractBackend
{

    //
    // defines

    /**
     * Path to the MODULE-DEF of the demo module
     */
    public static final String MODULE_SCHEMA_PATH = "/AUTOSAR/TOP-LEVEL-PACKAGES/TS_T00D0M0I0R0/ELEMENTS/GCDemo3";

    //
    // private members

    /**
     * The pushevent to reset the values on the tab "Stuff"
     */
    private ExampleAutoconfigureEvent m_pushEvent = null;

    @Override
    public List<APIOperationStatus> doRun()
    {
        // Prepare an error list
        ArrayList<APIOperationStatus> messages = new ArrayList<APIOperationStatus>();

        // Get the root memento
        Memento rootMemento = getMemento();

        // Do not run operations when checkbox is not set
        if (!rootMemento.getBoolean(Demo3Page.MEMENTOID_CHECKBOX))
        {
            return messages;
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

        // Return logged errors
        return m_pushEvent.getMessages();
    }

    /**
     * This method can be overwritten, to implement an own export of wizard results.
     *
     * <p>
     * The output printed by this method is written to exported log files or when running on the commandline. This demo
     * implementation prints the list of changed nodes.
     * </p>
     *
     * @param results The {@link List} of {@link APIOperationStatus} results of the wizard execution
     * @return a String containing the exported results
     * @see AbstractBackend#doExportResults(List)
     */
    @Override
    public String doExportResults(List<APIOperationStatus> results)
    {
        StringBuffer buffer = new StringBuffer();
        for (DCtxt changedNode : getChangedNodes())
        {
            buffer.append("Changed value of " + changedNode.getPath() + " to " + changedNode.var.getValue() + "\n");
        }
        return buffer.toString();
    }

    /**
     * Returns a list of DCtxts which have been changed
     *
     * @return list of changed {@link DCtxt}s
     */
    public List<DCtxt> getChangedNodes()
    {
        Vector<DCtxt> changedNodes = new Vector<DCtxt>();
        if (m_pushEvent == null)
        {
            return changedNodes;
        }
        for (TransactionLog log : m_pushEvent.getTransactionLogs())
        {
            changedNodes.add(log.getChangedDCtxt());
        }
        return changedNodes;
    }

    /**
     * Get the first configuration node of the project configuration to which this wizard belongs which matches the
     * given path in the schema (parameter definition).
     *
     * @param schemaPath The schema path
     * @return The referenced configuration node or null
     */
    private DCtxt getTargetConfigurationContext(String schemaPath)
    {
        ECUConfigContext ecuConfigContext = (ECUConfigContext)getContexts().get(ECUConfigContext.ID);
        List<DCtxt> dctxts = ecuConfigContext.getDCtxt().getFromSchema(schemaPath);
        return (dctxts != null) && (dctxts.size() > 0) ? dctxts.get(0) : null;
    }

} // Demo3Backend

