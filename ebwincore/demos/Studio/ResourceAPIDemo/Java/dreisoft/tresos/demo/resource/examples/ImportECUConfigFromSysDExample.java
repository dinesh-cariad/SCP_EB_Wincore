/*
 * Copyright (C) EB Automotive
 * All rights reserved
 */

package dreisoft.tresos.demo.resource.examples;

import dreisoft.tresos.autosar2.api.model.importer.ImporterConstants;
import dreisoft.tresos.datamodel2.api.model.DCtxt;
import dreisoft.tresos.datamodel2.api.model.Location;
import dreisoft.tresos.datamodel2.api.model.RCtxt;
import dreisoft.tresos.demo.resource.DemoBackend;
import dreisoft.tresos.demo.resource.DemoPlugin;
import dreisoft.tresos.demo.resource.generated.nls.ResourceAPIDemoNLS;
import dreisoft.tresos.demo.resource.generated.operationstatus.ResourceAPIDemoOperationStatus;
import dreisoft.tresos.guidedconfig.api.context.ECUConfigContext;
import dreisoft.tresos.lib2.api.log.APIOperationStatus;

import java.io.File;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;

/**
 * Example using the Resource API for importing ECU configuration data from system description into the current project.
 *
 * @since 11
 */
public class ImportECUConfigFromSysDExample extends ExampleBase
{

    /**
     * @param backend the demo backend that uses the example
     * @since 11
     */
    public ImportECUConfigFromSysDExample(DemoBackend backend)
    {
        super(backend, ResourceAPIDemoNLS.DESCRIPTION_IMPORT_SYSD_EXAMPLE());
    }

    /**
     * @since 11
     */
    private static final String INFILE = "resources/importECUConfigFromSysDExample/SystemWithComStackAutoConnect.arxml";

    /**
     * Imports ECU configuration data from an AUTOSAR system description into the current project using the public
     * Resource API.
     *
     * @param importFile the input file handle to import
     * @param status where status information is added
     * @since 2010.a
     */
    private void doImportECUConfig(File importFile, APIOperationStatus status)
    {
        // create a location from the input (ARXML) file handle with content auto detection
        Location locationIn = new Location(importFile, ImporterConstants.CONTENT_TYPE_SYSD + ":3.1.2");
        RCtxt rCtxt = RCtxt.create(locationIn);
        rCtxt.load(null, null, status);

        Map<String, String> options = new HashMap<String, String>();
        options.put(DCtxt.MERGE_SOURCE, "system");
        options.put(ImporterConstants.OPTION_VALIDATE, ImporterConstants.VAL_VALIDATE_FALSE);
        options.put(ImporterConstants.OPTION_INSTANCE_HANDLING, ImporterConstants.VAL_INSTANCE_SUFFIX_TRUE);
        options.put(ImporterConstants.OPTION_PROJECT, getDemoBackend().getECUConfigProjectName());
        options.put(ImporterConstants.OPTION_MODULE("EcuC"), "EcuC");
        options.put(ImporterConstants.OPTION_MODULE("PduR"), "PduR");
        options.put(ImporterConstants.OPTION_SYSTEM, "/System/TargetSystem");
        options.put(ImporterConstants.OPTION_ECU_INSTANCE, "/System/CEM");
        options.put(ImporterConstants.OPTION_BUFFER_ASSIGNMENT, ImporterConstants.VAL_BUFFER_ASSIGNMENT_TRUE);

        // load the data
        // This requires the project to have module configurations
        // for EcuC and PduR modules (named EcuC and PduR accordingly) plus proper
        // COM transformers registered (i.e. contributions to
        // the dreisoft.tresos.comimporter.api.plugin.comtransformer extension point).
        rCtxt.load(null, options, status);

        ECUConfigContext.getInstance().getDCtxt().merge(rCtxt, options, status);
    }

    /**
     * Run the import example.
     *
     * @param result to append status information onto
     * @throws Exception on any error
     * @since 2010.a
     */
    @Override
    public void run(ArrayList<APIOperationStatus> result) throws Exception
    {

        // prepare in and out file handles
        File inFile = DemoPlugin.getFileInBundle(INFILE);

        APIOperationStatus status = APIOperationStatus.getOkStatus();

        ///////////////////////////////
        // RESOURCE API IS USED HEREIN
        ///////////////////////////////
        doImportECUConfig(inFile, status);

        if (status.isOk())
        {
            // append some trace to be shown in the Results view
            result.add(ResourceAPIDemoOperationStatus.IMPORT_ECU_CONFIG_FROM_SYSD_EXAMPLE_RESULT(getDemoBackend().getECUConfigProjectName()));

            // make results visible
            // TODO needed
            // DemoBackend.refreshCurrentProject();
        }

        if (!status.isAbsolutelyOk())
        {
            result.add(status);
        }
    }

}
