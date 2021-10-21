/*
 * Copyright (C) Elektrobit Automotive GmbH
 * All rights reserved
 */
package dreisoft.tresos.guidedconfig.demo2;

import dreisoft.tresos.datamodel2.api.model.DCtxt;
import dreisoft.tresos.guidedconfig.api.memento.Memento;
import dreisoft.tresos.guidedconfig.api.pushservice.AbstractPushEvent;
import dreisoft.tresos.guidedconfig.api.pushservice.AbstractPushOperation;

import java.util.List;

/**
 * An example for a custom pushevent, used to set and reset the values for the tab "Stuff" of module configuration for
 * GCDemo2_TS_T00D0M0I0R0
 *
 * <p>
 * <i> Copyright (C) Elektrobit Automotive GmbH - All rights reserved</i>
 * </p>
 *
 * @since 2009.a
 */
public class ExamplePullEvent extends AbstractPushEvent
{

    //
    // defines

    /**
     * A {@link DCtxt} that references the target configuration that should be altered by the triggered
     * {@link AbstractPushOperation}.
     */
    public static final String VAR_TARGET_CONTEXT = "DContext";

    /**
     * The whole backend of the wizard.
     */
    public static final String VAR_BACKEND = "Backend";

    /**
     * The memento that forms the input to the PusOperation.
     */
    public static final String VAR_MEMENTO = "Memento";

    /**
     * The mementos that have been set from the configuration
     */
    public static final String VAR_CHANGED_MEMENTOS = "ChangedMementos";

    /**
     * Create a new Push Event that is sent to all {@link AbstractPushOperation}s that match this event.
     *
     */
    public ExamplePullEvent()
    {
        super();
    }

    /**
     * Set the memento from which to take the data.
     *
     * @param memento The memento of the wizard
     */
    public void setSourceMemento(Memento memento)
    {
        setVariable(VAR_MEMENTO, memento);
    }

    /**
     * Get the root memento of the wizard.
     *
     * @return The memento in the wizard
     */
    public Memento getSourceMemento()
    {
        return (Memento)getVariable(VAR_MEMENTO);
    }

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
     * Starts a transaction in {@link #getTargetContext()}
     */
    @Override
    protected void doPreOperationsHook()
    {
        super.doPreOperationsHook();
        getTargetContext().startTransaction("Example start operations");
    }

    /**
     * Finishes the transaction started in {@link #doPreOperationsHook()}
     */
    @Override
    protected void doPostOperationsHook()
    {
        super.doPostOperationsHook();
        getTargetContext().finishTransaction();
    }

    /**
     * Set the changed mementos.
     *
     * @param mementos The names of the changed mementos
     */
    public void setChangedMementos(List<String> mementos)
    {
        setVariable(VAR_CHANGED_MEMENTOS, mementos);
    }

    /**
     * Query the list of changed mementos.
     *
     * @return The changed mementos
     */
    @SuppressWarnings("unchecked")
    public List<String> getChangedMementos()
    {
        return (List<String>)getVariable(VAR_CHANGED_MEMENTOS);
    }

} // ExampleEvent
