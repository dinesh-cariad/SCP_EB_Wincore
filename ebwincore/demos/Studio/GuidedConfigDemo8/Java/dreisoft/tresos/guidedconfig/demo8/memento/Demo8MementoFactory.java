/*
 * Copyright (C) Elektrobit Automotive GmbH
 * All rights reserved
 */

package dreisoft.tresos.guidedconfig.demo8.memento;

import dreisoft.tresos.guidedconfig.api.memento.AbstractMementoFactory;
import dreisoft.tresos.guidedconfig.api.memento.Memento;
import dreisoft.tresos.guidedconfig.demo8.IConstants;

/**
 * This special implementation of the memento factory sets default values for some attributes of the root memento.
 */
public class Demo8MementoFactory extends AbstractMementoFactory
{

    /**
     * Singleton instance of the factory
     */
    public static final Demo8MementoFactory INSTANCE = new Demo8MementoFactory();

    /**
     * Private default constructor for the singleton class.
     */
    private Demo8MementoFactory()
    {
        super();
    }

    @Override
    public Memento getMemento(Memento parent, String name)
    {
        // let the superclass do its work
        Memento memento = super.getMemento(parent, name);
        // in this special case we only want to operate on the root memento
        // please note: since the memento is not yet fully created, you cannot use memento.isRootMemento() here
        if (parent != null)
        {
            return memento;
        }

        // set the default values for some attributes; the other memento attributes will not be initialized
        memento.setBoolean(IConstants.MEMENTO_ADD_PREFIX, false);
        memento.setString(IConstants.MEMENTO_PREFIX_TEXT, "");

        return memento;
    }

}
