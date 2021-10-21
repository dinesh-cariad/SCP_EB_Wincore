/*
 * Copyright (C) Elektrobit Automotive GmbH
 * All rights reserved
 */
package dreisoft.tresos.guidedconfig.demo8.pushservice;

import dreisoft.tresos.datamodel2.api.model.DCtxt;
import dreisoft.tresos.guidedconfig.api.memento.Memento;
import dreisoft.tresos.guidedconfig.api.pushservice.AbstractPushEvent;
import dreisoft.tresos.guidedconfig.api.pushservice.AbstractPushOperation;
import dreisoft.tresos.guidedconfig.demo8.backend.Demo8Backend;

/**
 * This push event is used to transfer data between the project data model {@link DCtxt} and the wizards {@link Memento}
 * s.
 *
 * The event is transfered between {@link Demo8Backend} and {@link Demo8PullOperation}. For the purpose of the data
 * transfer this subclass of {@link AbstractPushEvent} uses two variables. The variable {@link #VAR_DATA_CONTEXT} is
 * used to store the {@link DCtxt} instance and the variable {@link #VAR_MEMENTO} is used to store the {@link Memento}
 * instance. Both variables are set when an instance of this class is created by {@link Demo8Backend}. The value of the
 * variables is read by the classes {@link Demo8PullOperation} and {@link Demo8PushOperation}, which enables these push
 * operations to read and write {@link Memento} and {@link DCtxt} data. Please note: the event itself does not behave
 * differently depending on its direction.
 *
 *
 * <p>
 * <i> Copyright (C) Elektrobit Automotive GmbH - All rights reserved</i>
 * </p>
 *
 * @since 2010.a
 */
public class DataTransferEvent extends AbstractPushEvent
{

    //
    // defines

    /**
     * This constant can be used as name in the {@link #addCondition(String, Object)} method to set the direction of
     * this event.
     */
    public static final String EVENT_DIRECTION = "direction";

    /**
     * Value of the property {@link #EVENT_DIRECTION} if the event is used to write data into the data model
     * {@link DCtxt}.
     */
    public static final String EVENT_DIRECTION_PUSH = "push";

    /**
     * Value of the property {@link #EVENT_DIRECTION} if the event is used to read data from the data model
     * {@link DCtxt}.
     */
    public static final String EVENT_DIRECTION_PULL = "pull";

    /**
     * This variable is used to store the instance of {@link DCtxt} on which the corresponding
     * {@link AbstractPushOperation} shall operate.
     */
    private static final String VAR_DATA_CONTEXT = "DContext";

    /**
     * This variable is used to store the instance of {@link Memento} on which the corresponding
     * {@link AbstractPushOperation} shall operate.
     */
    private static final String VAR_MEMENTO = "Memento";

    /**
     * Create a new Push Event that is sent to all {@link AbstractPushOperation}s that match this event.
     */
    public DataTransferEvent()
    {
        super();
    }

    /**
     * Store the memento so that it can later be retrieved by {@link AbstractPushOperation}s that receive this event.
     *
     * @param memento The memento to store
     */
    public void setMemento(Memento memento)
    {
        setVariable(VAR_MEMENTO, memento);
    }

    /**
     * @return The stored memento
     */
    public Memento getMemento()
    {
        return (Memento)getVariable(VAR_MEMENTO);
    }

    /**
     * Store the data context so that it can later be retrieved by {@link AbstractPushOperation}s that receive this
     * event.
     *
     * @param context The context to store
     */
    public void setModuleConfigDCtxt(DCtxt context)
    {
        setVariable(VAR_DATA_CONTEXT, context);
    }

    /**
     * @return The stored context
     */
    public DCtxt getModuleConfigDCtxt()
    {
        return (DCtxt)getVariable(VAR_DATA_CONTEXT);
    }

} // DataTransferEvent
