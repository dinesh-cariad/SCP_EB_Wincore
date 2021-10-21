/*
 * Copyright (C) Elektrobit Automotive GmbH
 * All rights reserved
 */

package dreisoft.tresos.demo.resource.examples;

import dreisoft.tresos.demo.resource.DemoBackend;
import dreisoft.tresos.lib2.api.log.APIOperationStatus;

import java.util.ArrayList;

/**
 * Resource API demo example base.
 *
 * @since 11
 */
public abstract class ExampleBase
{

    /**
     * The description text of the example (may contain <code>HTML</code> tags.
     */
    private String m_description;

    private DemoBackend m_demoBackend;

    /**
     * @param backend the demo backend that uses the example
     * @param description a <code>HTML</code> string describing the example
     * @since 11
     */
    public ExampleBase(DemoBackend backend, String description)
    {
        m_demoBackend = backend;
        m_description = description;
    }

    /**
     * @return the description of the example (may contain <code>HTML</code> tags).
     * @since 11
     */
    public String getDescription()
    {
        return m_description;
    }

    /**
     * @return the demo backend instance
     * @since 11
     */
    protected DemoBackend getDemoBackend()
    {
        return m_demoBackend;
    }

    /**
     * Run the example.
     *
     * @param result
     * @since 11
     */
    abstract public void run(ArrayList<APIOperationStatus> result) throws Exception;

}