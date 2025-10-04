// Run after the entire page has been loaded

window.onload = (e) => {
    // Class name to search for
    const CLASS_NAME = "assets_price_diff_pct";
    // Column index of all `price_diff_pct` cells (same)
    let COL_IDX;

    // Get table body (`thead` and `tfoot` rows to be excluded)
    const assets = document.querySelector("table#portfolio").querySelector("tbody");

    // Iterate over all rows
    for (var i = 0, row; row = assets.rows[i]; i++) {
        for (var j = 0, cell; cell = row.cells[j]; j++) {
            if (cell.classList.contains(CLASS_NAME)) {
                COL_IDX = j;
                break;
            }
        }
    }
    for (var i = 0, row; row = assets.rows[i]; i++) {
        cell = row.cells[COL_IDX];  // use precomputed column index
        const prefix = cell.textContent[0];
        switch (prefix) {
            case "+": {
                cell.classList.add("positive");
                // cell.textContent = cell.textContent.slice(1);  // remove prefix
                break;
            }
            case "-": {
                cell.classList.add("negative");
                // cell.textContent = cell.textContent.slice(1);  // remove prefix
                break;
            }
            default: {
                // if cell only contains the character '~'
                if (cell.textContent.match(/\~/)){
                    cell.classList.add("zero");
                    break;
                }
            }
        }
     }
};
