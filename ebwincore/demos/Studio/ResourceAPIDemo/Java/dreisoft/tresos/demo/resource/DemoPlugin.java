/*
 * Copyright (C) Elektrobit Automotive GmbH
 * All rights reserved
 */

package dreisoft.tresos.demo.resource;

import org.eclipse.core.runtime.FileLocator;
import org.eclipse.core.runtime.Path;
import org.eclipse.ui.plugin.AbstractUIPlugin;
import org.osgi.framework.BundleContext;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.net.URL;

/**
 * The bundle activator of this plugin. Provides some utilities for the examples.
 *
 * @since 11
 */
public class DemoPlugin extends AbstractUIPlugin
{

    // The plug-in ID
    public static final String PLUGIN_ID = "ResourceAPIDemo";

    // The shared instance
    private static DemoPlugin s_plugin;

    /**
     * The default constructor
     */
    public DemoPlugin()
    {
    }

    /*
     * (non-Javadoc)
     *
     * @see org.eclipse.ui.plugin.AbstractUIPlugin#start(org.osgi.framework.BundleContext)
     */
    @Override
    public void start(BundleContext context) throws Exception
    {
        super.start(context);
        s_plugin = this;
    }

    /*
     * (non-Javadoc)
     *
     * @see org.eclipse.ui.plugin.AbstractUIPlugin#stop(org.osgi.framework.BundleContext)
     */
    @Override
    public void stop(BundleContext context) throws Exception
    {
        s_plugin = null;
        super.stop(context);
    }

    /**
     * @return the shared / singleton instance
     */
    public static DemoPlugin getDefault()
    {
        return s_plugin;
    }

    /**
     * Returns the file of the given path that is relative to the base folder of the bundle.
     *
     * @param path a relative path within this bundle
     * @return the requested file handle
     * @throws IOException if an error occurs during the conversion
     */
    public static File getFileInBundle(String path) throws IOException
    {
        URL url = FileLocator.find(getDefault().getBundle(), new Path(path), null);

        File f = new File(FileLocator.toFileURL(url).getFile());
        if (!f.exists())
        {
            throw new FileNotFoundException(path);
        }

        return f;
    }

}
