/*
 * Copyright (C) Elektrobit Automotive GmbH
 * All rights reserved
 */
package dreisoft.tresos.guidedconfig.demo3;

import dreisoft.tresos.datamodel2.api.model.DCtxt;
import dreisoft.tresos.datamodel2.api.model.TransactionLog;
import dreisoft.tresos.guidedconfig.api.pushservice.AbstractPushEvent;
import dreisoft.tresos.guidedconfig.api.pushservice.AbstractPushOperation;
import dreisoft.tresos.guidedconfig.api.pushservice.PushService;

import java.util.List;

/**
 * An example for a custom pushevent, used to set and reset the values for the tab "Stuff" of module configuration for
 * GCDemo3_TS_T00D0M0I0R0.xdm
 *
 * <p>
 * <i> Copyright (C) Elektrobit Automotive GmbH - All rights reserved</i>
 * </p>
 *
 * @since 2009.a
 */
public class ExampleAutoconfigureEvent extends AbstractPushEvent
{

    //
    // defines

    /**
     * A {@link DCtxt} that references the target configuration that should be altered by the triggered
     * {@link AbstractPushOperation}.
     */
    public static final String VAR_TARGET_CONTEXT = "DContext";

    /**
     * The nodes changed by the {@link AbstractPushOperation}
     */
    public static final String VAR_CHANGED_NODES = "ChangedNodes";

    /**
     * stores all {@link TransactionLog}s right before the transaction is finished to be returned in
     * {@link #getTransactionLogs()}
     */
    private List<TransactionLog> m_logs;

    /**
     * Set the target of the operation.
     *
     * @param target The target
     */
    public void setTargetContext(DCtxt target)
    {
        setVariable(VAR_TARGET_CONTEXT, target);
    }

    /**
     * Get the target of the operation
     *
     * @return The node in the project to change
     */
    public DCtxt getTargetContext()
    {
        return (DCtxt)getVariable(VAR_TARGET_CONTEXT);
    }

    /**
     * Set the nodes that have been changed. This method is used together with {@link #getChangedNodes()}, if this event
     * is not used within an transaction - see {@link #getTransactionLogs()}
     *
     * @param changedNodes The changed nodes
     * @see #getTransactionLogs()
     */
    public void setChangedNodes(List<DCtxt> changedNodes)
    {
        setVariable(VAR_CHANGED_NODES, changedNodes);
    }

    /**
     * Query the nodes that have been changed. This method is used together with {@link #setChangedNodes(List)}, if this
     * event is not used within an transaction - see {@link #getTransactionLogs()}
     *
     * @return [{@link DCtxt} of the nodes that have been changed by the {@link AbstractPushOperation}
     * @see #getTransactionLogs()
     */
    @SuppressWarnings("unchecked")
    public List<DCtxt> getChangedNodes()
    {
        return (List<DCtxt>)getVariable(VAR_CHANGED_NODES);
    }

    /**
     * clears the {@link TransactionLog}s (see {@link #getTransactionLogs()}) and starts a transaction for
     * {@link #getTargetContext()}
     */
    @Override
    protected void doPreOperationsHook()
    {
        if (m_logs != null)
        {
            m_logs.clear();
        }
        super.doPreOperationsHook();
        getTargetContext().startTransaction("Example start operations");
    }

    /**
     * reads out the {@link TransactionLog}s from the {@link #getTargetContext()} and finishes the transaction started
     * in {@link #doPreOperationsHook()}
     */
    @Override
    protected void doPostOperationsHook()
    {
        super.doPostOperationsHook();
        m_logs = getTargetContext().getTransactionLogs();
        getTargetContext().finishTransaction();
    }

    /**
     * returns all {@link TransactionLog}s of all operations done between {@link #doPreOperationsHook()} and
     * {@link #doPostOperationsHook()}. This does only work, if this event is used together with operations within an
     * Transaction: e.g. when used with {@link PushService#callSync(AbstractPushEvent, boolean)}
     *
     * @return all {@link TransactionLog}s of all operations done between {@link #doPreOperationsHook()} and
     *         {@link #doPostOperationsHook()}
     * @since 2010.a
     */
    public List<TransactionLog> getTransactionLogs()
    {
        return m_logs;
    }
} // ExampleAutoconfigureEvent
