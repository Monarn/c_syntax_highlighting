const express = require('express');
const {exec} = require('child_process')
const cors = require('cors')

app = express()
app.use(cors({
  origin: '*',
  credentials: true,
}))

const parseC = (req, res) => {
  const code = req.query.code;

  exec(`./c_parser/parser ${`"${decodeURIComponent(code)}"`}`, (error, stdout, stderr) => {

  res.status(200).send(`${stdout}`)
  })

}

app.get("/parse", parseC)

app.listen(3001, () => {
  console.log("Le serveur écoute de manière carrée")
})
